.code

ZwProtectVirtualMemory proc
	mov     r10, rcx        ; NtProtectVirtualMemory
	mov     eax, 50h ; '
	
	syscall 
	ret
ZwProtectVirtualMemory	endp
end

