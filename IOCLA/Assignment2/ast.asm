section .data
    idx dd 0

section .text

extern check_atoi
extern print_tree_inorder
extern print_tree_preorder
extern evaluate_tree
extern malloc

global create_tree
global iocla_atoi

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
iocla_atoi: 
    ; TODO
    push ebp
    mov ebp, esp
    push esi
    
    mov esi, dword [esp + 12]

    xor eax, eax
    xor ecx, ecx

    mov edx, 10
    cmp byte [esi], '-' ; verific daca numarul este negativ
    jne while

    mov ecx, 1 ; sar peste primul caracter ('-') daca exista

while: ; cat timp nu am ajuns la '\n'
    cmp byte [esi + ecx], 0x0
    je exit

    ; la fiecare pas: eax = eax * 10 + cifra_citita
    mul edx
    mov dl, byte [esi + ecx]
    sub dl, '0'    ; pun cifra in dl
    add eax, edx   ; o adaug la raspuns
    mov edx, 10    ; resetez edx la 10 pentru mul
    inc ecx        ; parcurg sirul
    jmp while

exit:
    cmp byte [esi], '-' ; verific daca numarul este negativ
    jne positive
    neg eax

positive:
    pop esi

    leave
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;char* copy(char* string)
;Functie ajutatoare similara cu strtok
;parcurge sirul primit ca parametru si returneaza un sir nou
;care contine primele elemente pana la primul ' ' sau '\0'
copy:
    push ebp
    mov ebp, esp
    xor eax, eax

    push 10
    call malloc ; aloc memorie pentru noul sir
    add esp, 4

    mov edx, dword [esp + 8]
    add edx, [idx]

    xor ecx, ecx

copy_while: ; cat timp nu am ajuns la ' ' sau '\0'
    cmp byte [edx + ecx], 0x0
    je copy_exit
    cmp byte [edx + ecx], 0x20
    je copy_exit

    push ebx
    mov bl, [edx + ecx]
    mov [eax + ecx], bl ; scriu in sirul de output
    pop ebx

    add dword [idx], 1
    inc ecx
    jmp copy_while

copy_exit:
    add dword [idx], 1
    mov byte [eax + ecx], 0x0 ; adaug '\0' la final
    leave 
    ret ; returnez noul sir

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
create_tree:
    ; TODO
    push ebp
    mov ebp, esp
    xor eax, eax

    mov esi, dword [esp + 8]
    
    push 12
    call malloc ; aloc memorie pentru o celula 
    add esp, 4

    mov ecx, eax

    push ecx
    push esi
    call copy ; citesc urmatorul sir din input (pana la ' ' sau '\0')
    pop esi
    pop ecx

    mov dword [ecx], eax

    cmp byte [eax], '0'      ; verific daca am citit un operator sau operand
    jge finish               ; daca primul caracter e cifra -> operand
    cmp byte [eax + 1], 0x0  ; daca al doilea caracter nu e '\0' -> operand
    jne finish               ; altfel -> operator
    
    push ecx
    push esi ; pun inputul inapoi pe stiva
    call create_tree ; apelez recursiv pentru nodul stanga
    pop esi 
    pop ecx
    mov dword [ecx + 4], eax ; il pun la adresa corespunzatoare stanga

    push ecx
    push esi ; pun inputul inapoi pe stiva
    call create_tree ; apelez recursiv pentru nodul dreapta
    pop esi
    pop ecx
    mov dword [ecx + 8], eax ; il pun la adresa corespunzatoare dreapta

finish:
    mov eax, ecx ; returnez pointer la nodul creat
    leave
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
