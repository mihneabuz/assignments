%include "io.mac"

section .text
    global caesar
    extern printf

section .data
	len DD 0

caesar:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     edi, [ebp + 16]     ; key
    mov     ecx, [ebp + 20]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement the caesar cipher

	mov ebx, -1                 ; folosesc ebx ca index pentru parcurgere
	mov [len], ecx              ; salvez in memorie len ca sa pot folosi ecx
    mov ecx, edi 				; mut key ca sa il pot folosi dintr-un 
	mov byte ch, 26 			; registru pe 1 byte (ch)

loop:
	inc ebx                     ; incrementez indexul
	cmp ebx, [len]              ; verific daca am term stringul
	je end

	mov eax, 0
	mov al, [esi + ebx]         ; pun char curent in al

	cmp byte al, 90             ; verific daca poate fi majuscula
	jle majuscula

	cmp byte al, 122            ; verific daca poate fi minuscula
	jle minuscula

	jmp write                   ; daca nu, scriu caracterul

majuscula:
	cmp byte al, 65             ; daca nu e majuscula, scrie direct
	jl write
	; 65 <= al <= 90
	sub al, 65                  ; 
	add al, cl                  ; calculez noul caracter
    div ch                      ; 
	add ah, 65                  ;
	mov byte [edx + ebx], ah    ; il scriu in output
	jmp loop

minuscula:
	cmp byte al, 97             ; daca nu e minuscula, scrie direct
	jl write
	; 97 <= al <= 122
	sub al, 97                  ; 
	add al, cl                  ; calculez noul caracter
    div ch                      ;
    add ah, 97                  ;
    mov byte [edx + ebx], ah    ; il scriu in output
	jmp loop

write:                          ; scrie acelasi caracter in output
	mov byte [edx + ebx], al
	jmp loop

end:
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
