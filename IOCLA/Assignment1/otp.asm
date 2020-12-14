%include "io.mac"

section .text
    global otp
    extern printf

otp:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     edi, [ebp + 16]     ; key
    mov     ecx, [ebp + 20]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement the One Time Pad cipher

	mov ebx, 0;         ; parcurg cu index ebx

repeta:
	mov al, [esi + ebx] ; pun caracterul de pe poz ebx in al
	xor al, [edi + ebx] ; fac xor pe al
	mov [edx + ebx], al ; mut caracterul shimbat in ciphertext
	inc ebx             ; ebx ++
	loop repeta         
	
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
