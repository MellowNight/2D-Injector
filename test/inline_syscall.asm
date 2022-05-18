PUBLIC _syscaller_stub
     
.code
     
_syscaller_stub PROC
    mov r10, rcx
    pop rcx
    pop rax
    mov QWORD PTR [rsp], rcx
    mov eax, [rsp + 24]
    syscall
    sub rsp, 8
    jmp QWORD PTR [rsp + 8]
_syscaller_stub ENDP
     
END