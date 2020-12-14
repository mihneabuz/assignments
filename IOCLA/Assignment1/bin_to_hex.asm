%include "io.mac"

section .text
    global bin_to_hex
    extern printf

section .data
	len DD 0	

bin_to_hex:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; hexa_value
    mov     esi, [ebp + 12]     ; bin_sequence
    mov     ecx, [ebp + 16]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement bin to hex

	mov [len], ecx              ;
	mov ecx, 0                  ; pun lungimea in memorie ca sa eliberez reg
	mov eax, 0                  ; initializez registrii cu 0
	mov ebx, 0                  ;
	
	mov word ax, [len]          ;
	mov cl, 4                   ; caluclez in functie de lungime, cati biti
	div cl                      ; va avea de citit prima cifra in hexa
	mov bh, 4                   ;
	sub bh, ah                  ; ex len 11 -> prima cifra in hexa -> 3biti
                                ; -> bh incepe de la 1
	cmp bh, 4
	jl special                  ; caz special daca len % 4 == 0 -> bh incepe 0
	mov bh, 0
special:

	mov ecx, -1	                ; cu ecx parcurg bitii
	mov eax, 0                  ; cu eax parcurg sirul de hexa

loop:
	inc ecx
	cmp ecx, [len]              ; cand ajung la finalul bitilor, termin
	je end

	inc bh
	sub byte [esi + ecx], 48	; transform din char in numar
	add byte bl, [esi + ecx]    

	cmp bh, 4                   ; cat timp nu am scris 4 biti
	jl skip                     ; continui sa citesc

	mov bh, 0                   ; daca am scris 4 biti
	cmp bl, 10                  ; verific daca am obtinut o cifra < 10
	jl digit                    ; daca da sar la digit

	add bl, 7                   ; daca nu mai adaug (65 - 48 - 9) = 7
                                ; ca sa devina litera mare
digit:
	add bl, 48                  ; daca e cifra adaug 48

	mov byte [edx + eax], bl    ; adaug la output
	inc eax	                    ; incrementez indexul outputului
	mov bl, 0                   ; sterg val din bl

skip:
	shl bl, 1                   ; shiftez bl la stanga ca sa compun numarul
	jmp loop

end:
	mov byte [edx + eax], 10    ; adaug newline la final
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
