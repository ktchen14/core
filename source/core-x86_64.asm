; vim: set ft=nasm:

; ELF and Mach-O have extensions to the GLOBAL directive
%ifidn __?OUTPUT_FORMAT?__, elf64
  %define suffix(name) :function (name %+ .tail - name) hidden
%elifidn __?OUTPUT_FORMAT?__, macho64
  %define suffix(name) :private_extern
%else
  %define suffix(name)
%endif

; Define the symbol macro
%macro symbol 1
  global %{1} %+ suffix(%1)
  static %{1}.tail
  align 16
  %1:
%endmacro

extern core_return

SECTION .text

; void *estate_switch(
;   estate_t *target <rdi>, void *result <rsi>, estate_t *source <rdx>)
symbol estate_switch
  ; Save rbx, rbp, r12, r13, r14, r15, the x87 control word, and the MXCSR
  ; register state onto the stack.
  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15
  fnstcw [rsp - 0x08]
  stmxcsr [rsp - 0x0A]

  ; *source [rdx] = rsp
  mov [rdx], rsp

  ; rsp = *target [rdi]
  mov rsp, [rdi]

  ; Load rbx, rbp, r12, r13, r14, r15, the x87 control word, and the MXCSR
  ; register state from the stack.
  ldmxcsr [rsp - 0x0A]
  fldcw [rsp - 0x08]
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx

  ; return = result
  mov rax, rsi
  ret
.tail:

symbol entrance_hook
  mov rdi, rsi
  ret
.tail:

symbol return_hook
  mov rdi, rax
  add rsp, 8
  jmp core_return wrt ..plt
.tail:

symbol bounce_hook
  ret
.tail:

%pragma elf    gprefix
%pragma output gprefix _

; Missing .note.GNU-stack section implies executable stack
%ifidn __?OUTPUT_FORMAT?__, elf64
  SECTION .note.GNU-stack note noalloc noexec nowrite align=4
%endif
