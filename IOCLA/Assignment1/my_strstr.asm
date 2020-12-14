%include "io.mac"

section .text
    global my_strstr
    extern printf

section .data
	len DD 0 

my_strstr:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edi, [ebp + 8]      ; substr_index
    mov     esi, [ebp + 12]     ; haystack
    mov     ebx, [ebp + 16]     ; needle
    mov     ecx, [ebp + 20]     ; haystack_len
    mov     edx, [ebp + 24]     ; needle_len
    ;; DO NOT MODIFY

    ;; TO DO: Implement my_strstr

	mov eax, 0                  ;seteaza eax la 0
	mov dword [len], edx              ;salveaza lungimea subsirului in memorie
	mov edx, 0                  ;refolosesc edx pentru iterare

	mov al, [esi]               ;verific primul caracter
	cmp al, [ebx]               ;
	je set_edi	                ;daca sunt identice salvez pozitia si verific
	
	
not_found:                      ;loop pentru a gasii primul caracter din subsir
	inc edx                     ;incrementez indexul curent la sirului
	inc esi                     ;trec la urm caracter din sir
	cmp byte [esi], 0x0         ;verific daca am ajuns la \0
	je bad_end                  ;daca da, sar la bad_end
	mov al, [esi]               ;verific caracterul curent cu primul din subsir
	cmp al, [ebx]               ;
	jne not_found               ;daca sunt diferite revin la loop
	
set_edi:
	mov [edi], edx              ;salvez pozitia primului char

found:                          ;loop pentru a verifica restul char din subsir
	inc edx                     ;incrementez indexul curent al sirului
	inc ebx                     ;trec la urm caracter din subsir
	inc esi                     ;trec la urm caracter din sir
 
	cmp byte [ebx], 0x0         ;verific daca am ajuns la finalul subsirului
	je end                      ;daca da, am gasit prima aparitie

	cmp byte [esi], 0x0         ;verific daca am ajuns la finalul sirului
	je bad_end                  ;daca da, sar la bad_end

	mov al, [esi]               ;verific daca char din sir = char din subsir
	cmp al, [ebx]               ;
	je found                    ;daca da, reiau loop-ul found
								
	add ebx, [edi]              ;daca nu, reiau subsirul de la inceput
	sub ebx, edx                ;
	jmp not_found               ;trec inapoi la loop-ul not_found

bad_end:                        ;daca nu gasesc subsirul
	inc ecx
	mov [edi], ecx

end:                            ;daca gasesc subsirul
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
