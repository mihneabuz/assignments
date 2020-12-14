%include "io.mac"

section .text
    global vigenere
    extern printf

section .data
	key_len DD 0    
	plaintext_len DD 0

vigenere:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     ecx, [ebp + 16]     ; plaintext_len
    mov     edi, [ebp + 20]     ; key
    mov     ebx, [ebp + 24]     ; key_len
    ;; DO NOT MODIFY

    ;; TODO: Implement the Vigenere cipher

	mov dword [plaintext_len], ecx
    mov dword [key_len], ebx    ; salvez lungimile ca sa refolosesc ecx si ebx

	mov eax, -1
	mov ebx, -1

extend_key:                     ; loop care extinde cheia si o scrie in [edx]
	inc eax
	inc ebx
	cmp eax, [plaintext_len]
	je cipher

	cmp ebx, [key_len]          ;cand ajung la sf key-ului, il reiau de la poz 0
	jl skip
	mov ebx, 0 
skip:

	mov byte cl, [esi + eax]    ; pun char curent in cl
	
	cmp cl, 65
	jl special_character        ; verific ce fel de char este

	cmp cl, 122
	jg special_character

	cmp cl, 90
	jle encryptable

	cmp cl, 97
	jge encryptable

	jmp special_character		

encryptable:                    ; char encryptable
	mov byte cl, [edi + ebx]
	mov byte [edx + eax], cl
	jmp extend_key

special_character:              ; char speciale/ neencryptabile
	sub ebx, 1
	mov byte cl, [esi + eax]
	mov byte [edx + eax], cl
	jmp extend_key 

cipher:                         ; initializez registrele pentru encryptare
	mov ebx, -1
	mov eax, 0
	mov ecx, 0
	mov ch, 26

cipher_loop:                    ; loop-ul de encryptare
	inc ebx
	cmp ebx, [plaintext_len]
	je end

	mov eax, 0
	mov al, [esi + ebx]         ; pun carcterul curent de encryptat in al
	mov cl, [edx + ebx]         ; pun cheia curenta de encryptare in cl

	cmp byte al, 90             ; verific ce fel de carcater e
	jle majuscula

	cmp byte al, 122
	jle minuscula

	jmp cipher_loop             ; daca nu e litera, il ignor

majuscula:
	cmp byte al, 65             ; daca e majuscula
	jl cipher_loop

	sub al, 65                  ;
	sub cl, 65                  ; culculez litera encryptata
	add al, cl                  ;
	div ch                      ;
	add ah, 65                  ;

	mov byte [edx + ebx], ah    ; o scriu in string
	jmp cipher_loop
	
minuscula:
	cmp byte al, 97             ; daca e minuscula
	jl cipher_loop

	sub al, 97                  ;
	sub cl, 65                  ; calculez litera encryptata
	add al, cl                  ;
	div ch                      ;
	add ah, 97                  ;

	mov byte [edx + ebx], ah    ; o scriu in string
	jmp cipher_loop

end:
	;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
