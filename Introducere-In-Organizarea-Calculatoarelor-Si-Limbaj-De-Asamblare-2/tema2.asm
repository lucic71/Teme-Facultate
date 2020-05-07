; Tema2 IOCLA - Popescu Lucian Ioan 321CD

%include "include/io.inc"

extern atoi
extern printf
extern exit

; ---> my exports
extern calloc
extern free

; Functions to read/free/print the image.
; The image is passed in argv[1].
extern read_image
extern free_image
; void print_image(int* image, int width, int height);
extern print_image

; Get image's width and height.
; Store them in img_[width, height] variables.
extern get_image_width
extern get_image_height

section .rodata
    NULL equ 0x00
    STACK_WORD equ 0x04
    MAX_KEY equ 0xFF

    PrintStringFormat db "%s", 0x0A, 0x00
    PrintIntegerFormat db "%d", 0x0A, 0x00

    MessageToSend db "C'est un proverbe francais.", 0x00
    MessageToSendLength equ $ - MessageToSend

    MorseCodeBinaryTree db "**ETIANMSURWDKGOHVF*L*PJBXCYZQ**54*3***2*******16*"
    db "******7***8*90", 0x00
    MorseCodeBinaryTreeLength equ $ - MorseCodeBinaryTree

section .data
	use_str db "Use with ./tema2 <task_num> [opt_arg1] [opt_arg2]", 10, 0

section .bss
    task:       resd 1
    img:        resd 1
    img_width:  resd 1
    img_height: resd 1

section .text
global main
main:
    ; Prologue
    ; Do not modify!
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    cmp eax, 1
    jne not_zero_param

    push use_str
    call printf
    add esp, STACK_WORD

    push -1
    call exit

not_zero_param:
    ; We read the image. You can thank us later! :)
    ; You have it stored at img variable's address.
    mov eax, [ebp + 12]
    push DWORD[eax + 4]
    call read_image
    add esp, STACK_WORD
    mov [img], eax

    ; We saved the image's dimensions in the variables below.
    call get_image_width
    mov [img_width], eax

    call get_image_height
    mov [img_height], eax

    ; Let's get the task number. It will be stored at task variable's address.
    mov eax, [ebp + 12]
    push DWORD[eax + 8]
    call atoi
    add esp, STACK_WORD
    mov [task], eax

    ; There you go! Have fun! :D
    mov eax, [task]
    cmp eax, 1
    je solve_task1
    cmp eax, 2
    je solve_task2
    cmp eax, 3
    je solve_task3
    cmp eax, 4
    je solve_task4
    cmp eax, 5
    je solve_task5
    cmp eax, 6
    je solve_task6
    jmp done

solve_task1:
    push ebp
    mov ebp, esp

    ; OFFSET:
    ; -4 the encoded message
    sub esp, 4

    ; Allocate space to store a xor'ed line. In this block of memory
    ; we will search for 'revient'.
    mov eax, [img_width]
    inc eax ; +1 byte for the string terminator.

    ; Allocate <EAX> blocks of size 1
    push dword 1
    push eax
    call calloc
    add esp, STACK_WORD * 2

    mov [ebp - 4], eax

    push dword [ebp - 4]
    push dword [img]
    call bruteforce_singlebyte_xor
    add esp, 8

PrintMessKeyLine:
    ; Save the return value of bruteforce_singlebyte_xor.
    push eax

    ; Print the message.
    push dword [ebp - 4]
    push PrintStringFormat
    call printf
    add esp, STACK_WORD * 2

    ; Free the space allocated for a xor'ed line.
    push dword [ebp - 4]
    call free
    add esp, STACK_WORD

    ; Print the key.
    mov eax, [esp]
    shr eax, 16

    push eax
    push PrintIntegerFormat
    call printf
    add esp, STACK_WORD * 2

    ; Print the line.
    pop eax
    and eax, 0x0000FFFF

    push eax
    push PrintIntegerFormat
    call printf
    add esp, STACK_WORD * 2

task1_done:
    mov esp, ebp
    pop ebp
    jmp done
solve_task2:
    push ebp
    mov ebp, esp

    ; OFFSET:
    ; -4 the encoded message
    ; -8 the key and the line
    ; -12 the size of the matrix (width * height)
    sub esp, 12

    ; Allocate space to store a xor'ed line. In this block of memory
    ; we will search for 'revient'.
    mov eax, [img_width]
    inc eax ; +1 byte for the string terminator.

    ; Allocate <EAX> blocks of size 1
    push dword 1
    push eax
    call calloc
    add esp, STACK_WORD * 2

    mov [ebp - 4], eax

    push dword [ebp - 4]
    push dword [img]
    call bruteforce_singlebyte_xor
    add esp, 8

    ; Save the key and the line.
    mov [ebp - 8], eax

    ; Free the space allocated for the decoded message.
    push dword [ebp - 4]
    call free
    add esp, STACK_WORD

    ; Matrix base pointer.
    mov ebx, [img]

    ; Index used to iterate through matrix.
    mov ecx, [img_width]
    imul ecx, [img_height]

    mov [ebp - 12], ecx

    ; Key used to xor each cell in the matrix.
    mov edx, [ebp - 8]
    shr edx, 16

DecryptMatrix:
    ; Xor the entry and save it back at the same position.
    mov eax, [ebx]
    xor eax, edx
    mov [ebx], eax

    ; Go th the next entry in the matrix.
    add ebx, 4

    ; Decrement the index.
    dec ecx
    cmp ecx, NULL
    jnz DecryptMatrix

GetAddressForWriting:
    ; Get the line to write the message on
    mov eax, [ebp - 8]
    and eax, 0x0000FFFF
    inc eax

    ; Move the pointer to the following position:
    ; (img_width * (line + 1) * 4).
    mov edi, [img]
    imul eax, [img_width]
    imul eax, 4

    add edi, eax

    ; Move in <ESI> the message to be written because we will use lodsb.
    mov esi, MessageToSend

    ; Index for the WriteMessageToLineLoop loop
    mov ecx, MessageToSendLength

    ; <AL> will load bytes from MessageToSend with lodsb.
    ; <EDI> will store doublewords from eax with lodsd.
    xor eax, eax
WriteMessageToLineLoop:
    lodsb
    stosd

    dec ecx
    cmp ecx, NULL
    jnz WriteMessageToLineLoop

GenerateNewKey:
    mov eax, [ebp - 8]
    shr eax, 16

    shl eax, 1
    add eax, 3

    mov ecx, 5
    xor edx, edx
    div ecx

    sub eax, 4

    ; <EDX> will contain the key used to encrypt the matrix.
    mov edx, eax

    ; Matrix base pointer.
    mov ebx, [img]

    ; Matrix counter.
    mov ecx, [ebp - 12]

EncryptMatrix:
    ; Xor the entry with the new generated key.
    mov eax, [ebx]
    xor eax, edx
    mov [ebx], eax

    ; Go to the next entry in the matrix.
    add ebx, 4

    ; Decrement the index.
    dec ecx
    cmp ecx, NULL
    jnz EncryptMatrix

    push dword [img]
    call PrintImageWrapper
    add esp, STACK_WORD

task2_done:
    mov esp, ebp
    pop ebp
    jmp done

solve_task3:
    ; Points to the beginning of 'argv'.
    mov ebx, [ebp + 12]

    push ebp
    mov ebp, esp

    mov eax, [ebx + 16]
    push eax
    call atoi
    add esp, STACK_WORD

    push eax
    push dword [ebx + 12]
    push dword [img]
    call morse_encrypt
    add esp, 12

    mov esp, ebp
    pop ebp

    jmp done
solve_task4:
    ; Points to the beginning of 'argv'.
    mov ebx, [ebp + 12]

    push ebp
    mov ebp, esp

    mov eax, [ebx + 16]
    push eax
    call atoi
    add esp, STACK_WORD

    push eax
    push dword [ebx + 12]
    push dword [img]
    call lsb_encode
    add esp, STACK_WORD * 3

    mov esp, ebp
    pop ebp

    jmp done
solve_task5:
    ; Points to the beginning of 'argv'.
    mov ebx, [ebp + 12]

    push ebp
    mov ebp, esp

    mov eax, [ebx + 12]
    push eax
    call atoi
    add esp, STACK_WORD

    push eax
    push dword [img]
    call lsb_decode
    add esp, STACK_WORD * 2

    mov esp, ebp
    pop ebp

    jmp done
solve_task6:
    push ebp
    mov ebp, esp

    push dword [img]
    call blur
    add esp, STACK_WORD

    mov esp, ebp
    pop ebp

    jmp done

done:
    ; Free the memory allocated for the image.
    push DWORD[img]
    call free_image
    add esp, STACK_WORD

    ; Epilogue
    ; Do not modify!
    xor eax, eax
    leave
    ret

    ; #arg1: image to process
    ; #arg2: a string that will contain the searched text at the end
    ;   of this subroutine
    ;
    ; #returns: the key in the high part of <EAX> and the line in the low
    ;   part of <EAX>
    ;
    ; modified registers: <EAX>, <EBX>, <ECX>, <EDX>, <EDI>
bruteforce_singlebyte_xor:
    push ebp
    mov ebp, esp

    ; OFFSET:
    ; -4 saves the key used in bruteforce.
    ; -8 saves a pointer to a xor'ed line.
    sub esp, 8

    mov eax, [ebp + 12]
    mov [ebp - 8], eax

    xor eax, eax ; Used for arithmetic operations.
    mov [ebp - 4],eax ; Key stored for bruteforce.

    xor ecx, ecx ; Iterator through lines.
    xor edx, edx ; Iterator through columns.

BruteforceKeyLoop:
    mov ebx, [ebp + 8] ; matrix base pointer

LineTraversal:
    ; Put in <EDI> to store the xor'ed bytes using stosb.
    mov edi, [ebp - 8]

    ; restore column number to 0
    xor edx, edx
ColumnTraversal:
    mov eax, [ebx + edx * 4]

    ; Xor with the key and store the result in <EDI>
    xor eax, [ebp - 4]
    stosb

    inc edx
    cmp edx, [img_width]
    jl ColumnTraversal

MessageCheck:
    ; Save <EAX> and <EDX>.
    push eax
    push edx

    ; 'revient' string
    push dword 0x00746e65
    push dword 0x69766572

    push esp
    push dword [ebp - 8]
    call strstr

    ; Delete subroutine parameters.
    add esp, STACK_WORD * 2
    ; Delete 'revient' string.
    add esp, STACK_WORD * 2
    pop edx

    cmp eax, NULL
    pop eax
    jz MessageNotFound

MessageFound:
    ; Save the key in the high part of <EAX>(bits 31-16).
    mov eax, [ebp - 4]
    shl eax, 16

    ; Save the line in the low part of <EAX>(bits 15-0)
    mov ax, cx

    jmp bruteforce_singlebyte_xor_done

MessageNotFound:
    ; Move matrix base pointer to the beginning of the next line.
    mov eax, [img_width]
    imul eax, 4
    add ebx, eax
    xor eax, eax

    inc ecx

    cmp ecx, [img_height]
    jl LineTraversal

LineTraversalDone:
    ; Restore the line counter.
    xor ecx, ecx

BruteforceKeyIncrement:
    inc byte [ebp - 4]

    mov eax, [ebp - 4]
    cmp eax, MAX_KEY
    jl BruteforceKeyLoop

bruteforce_singlebyte_xor_done:
    mov esp, ebp
    pop ebp

    ret

    ; #arg1: haystack
    ; #arg2: needle
    ;
    ; #returns: NULL if needle is not found in haystack
    ;   else the beginning of the needle in the haystack
    ; modified registers: <EAX>, <ESI>, <EDI>
strstr:
    push ebp
    mov ebp, esp

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]

HaystackLoop:
    mov al, [esi]

    ; *haystack != 0
    test al, al
    jz ReturnNull

    ; *haystack != *needle
    cmp al, [edi]
    jnz HaystackIncrement

    ; compare(haystack, needle) == true
    push edi
    push esi
    call HaystackNeedleCompare
    pop esi
    pop edi

    test al, al
    jz HaystackIncrement

    mov eax, esi
    jmp strstrExit

HaystackIncrement:
    inc esi
    jmp HaystackLoop

ReturnNull:
    xor eax, eax

strstrExit:
    mov esp, ebp
    pop ebp

    ret

    ; #arg1: haystack
    ; #arg2: needle
    ;
    ; #returns: true if the first len(needle) bytes from arg1 and arg2 are
    ;   equal else NULL
    ; modified registers: <EAX>, <EDX>, <ESI>, <EDI>
HaystackNeedleCompare:
    push ebp
    mov ebp, esp

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]

CompareLoop:
    ; *haystack
    mov al, [esi]

    ; *needle
    mov dl, [edi]

    ; loop while *haystack != 0 and *needle != 0
    test al, al
    jz NeedleFinalCheck

    test dl, dl
    jz NeedleFinalCheck

    ; *haystack != *needle
    cmp al, dl
    jz HaystackNeedleIncrement

    xor eax, eax
    jmp CompareReturn

HaystackNeedleIncrement:
    inc esi
    inc edi

    jmp CompareLoop

NeedleFinalCheck:
    ; return *needle == NULL
    neg dl
    sbb dl, dl
    neg dl
    xor dl, 1

    mov eax, edx

CompareReturn:
    mov esp, ebp
    pop ebp

    ret

    ; #arg1: image to be processed
    ; #arg2: message to be encoded
    ; #arg3: index of the first byte to be changed in the image
    ;
    ; #returns: nothing
    ; modified registers: <EAX>, <EBX>, <ECX>, <EDX>, <ESI>, <EDI>
morse_encrypt:
    push ebp
    mov ebp, esp

    ; OFFSETS
    ; -4 number of pushes on stack in EncodeSymbol label
    sub esp, 4

    ; Initialize the number of pushes to 0.
    mov [ebp - 4], dword NULL

    ; Save the string to encode in <ESI>
    mov esi, [ebp + 12]

    ; Pointer to the beginning of where the message will be written.
    mov ebx, [ebp + 8]
    mov eax, [ebp + 16]

    imul eax, 4
    add ebx, eax

    xor eax, eax
MessageToEncodeLoop:
    lodsb

    ; If we encounter the string terminator then stop.
    test al, al
    jz MessageToEncodeLoopDone

    ; If we encounter a ',' then write '--..--'
    cmp al, 0x2C
    jnz WriteNormalEncoding

WriteCommaEncoding:
    ; -
    mov al, 0x2D
    mov [ebx], eax
    add ebx, 4

    ; -
    mov al, 0x2D
    mov [ebx], eax
    add ebx, 4

    ; .
    mov al, 0x2E
    mov [ebx], eax
    add ebx, 4

    ; .
    mov al, 0x2E
    mov [ebx], eax
    add ebx, 4

    ; -
    mov al, 0x2D
    mov [ebx], eax
    add ebx, 4

    ; -
    mov al, 0x2D
    mov [ebx], eax
    add ebx, 4

    jmp WriteSpaceAfterSymbol

WriteNormalEncoding:
    ; Search for the letter in MorseCodeBinaryTree using scasb
    mov ecx, MorseCodeBinaryTreeLength
    mov edi, MorseCodeBinaryTree

    cld
    repne scasb

    dec edi

    ; Calculate the index of that symbol in the tree which is represented as an array.
    mov eax, MorseCodeBinaryTree
    sub edi, eax

    mov eax, edi

    ; The symbol '*' in the MorseCodeBinaryTree is not a valid entry. To encode
    ; a symbol, we need to perform multiple divisions by 2 since for each node
    ; n, the left child will be on position 2*n and the right child will be on
    ; position 2*n + 1. As we go to the right we write '-'. As we go to the left
    ; we write '.'.
EncodeSymbol:
    cmp eax, 1
    jz EncodeSymbolDone

    mov ecx, 2
    xor edx, edx

    div ecx
    cmp edx, 1
    jz PushStackDash

PushStackDot:
    push dword 0x2E
    inc dword [ebp - 4]
    jmp PushStackDone

PushStackDash:
    push dword 0x2D
    inc dword [ebp - 4]

PushStackDone:

    jmp EncodeSymbol
EncodeSymbolDone:

    ;Write the encoded symbols to the desired position in the image.
WriteSymbolInsideImage:
    pop eax

    mov [ebx], eax
    add ebx, 4

    dec dword [ebp - 4]
    cmp [ebp - 4], dword NULL
    jnz WriteSymbolInsideImage

    ; If we are not at the last element in the string to be encoded then we
    ; write a space after we wrote a symbol.
WriteSpaceAfterSymbol:
    cmp [esi], byte NULL
    jz MessageToEncodeLoopReturn

    mov eax, 0x20
    mov [ebx], eax
    add ebx, 4

MessageToEncodeLoopReturn:
    xor eax, eax
    jmp MessageToEncodeLoop

MessageToEncodeLoopDone:
    ; Write a NULL byte at the end of the encoding.
    mov [ebx], dword NULL

    push dword [img]
    call PrintImageWrapper
    add esp, STACK_WORD

    mov esp, ebp
    pop ebp

    ret

    ; #arg1: image to be processed
    ; #arg2: message to be encoded
    ; #arg3: starting index for encoding
    ;
    ; #returns: nothing
    ; modified registers: <EAX>, <EBX>, <EDX>, <ESI>
lsb_encode:
    push ebp
    mov ebp, esp

    ; OFFSETS
    ; -4 cotains the current encoded byte

    ; Save the string to be encoded in <ESI>
    mov esi, [ebp + 12]

    ; Move the matrix base pointer with byte_id positions.
    mov eax, [ebp + 16]

    dec eax
    imul eax, 4

    mov ebx, [ebp + 8]
    add ebx, eax

    xor eax, eax
WriteLSBLoop:
    lodsb
    mov [ebp - 4], eax

    mov ecx, 8
ShiftBitToCarry:
    shl al, 1
    jnc SetLSBZero

SetLSBOne:
    mov edx, [ebx]
    or edx, 0x01
    mov [ebx], edx
    add ebx, 4

    jmp ShiftBitToCarryReload

SetLSBZero:
    mov edx, [ebx]
    and edx, 0xFE
    mov [ebx], edx
    add ebx, 4

ShiftBitToCarryReload:
    loop ShiftBitToCarry

    mov al, [ebp - 4]
    test al, al
    jnz WriteLSBLoop

    push dword [img]
    call PrintImageWrapper
    add esp, STACK_WORD

    mov esp, ebp
    pop ebp

    ret

    ; #arg1: image to be processed
    ; #arg2: starting index of the message to be decoded
    ;
    ; #returns: nothing
    ; modified registers: <EAX>, <EBX>, <ECX>, <EDX>, <EDI>
lsb_decode:
    push ebp
    mov ebp, esp

    ; OFFSETS
    ; -4 address of the decoded string
    sub esp, 4

    ; Move matrix pointer to the desired index
    mov eax, [ebp + 12]
    dec eax
    imul eax, 4

    mov ebx, [ebp + 8]
    add ebx, eax

    ; Allocate space for the decoded message.
    push dword 1
    push dword 21
    call calloc
    add esp, STACK_WORD * 2

    mov edi, eax
    mov [ebp - 4], eax

    ; Loop through 20 * 8 bytes because 20 is the maximum length of the
    ; message and 8 is the number of bits in a byte.
    mov ecx, 20

BytesIterator:
    push ecx
    mov ecx, 7

    ; <EDX> will be used to store each 8-bit sequence
    xor edx, edx
FetchLSB:
    mov eax, [ebx]
    add ebx, 4

    and eax, 0x01
    shl eax, cl

    or edx, eax

    dec ecx
    cmp ecx, NULL
    jge FetchLSB

FetchLSBDone:

    mov eax, edx
    stosb

    pop ecx
    dec ecx

    cmp ecx, NULL
    jnz BytesIterator
BytesIteratorDone:

DecodedMessagePrint:
    push dword [ebp - 4]
    push PrintStringFormat
    call printf
    add esp, STACK_WORD * 2

MemoryFree:
    mov eax, [ebp - 4]

    push eax
    call free
    add esp, STACK_WORD

    mov esp, ebp
    pop ebp

    ret

    ; #arg1: image to be processed
    ;
    ; #returns: nothing
    ; modified registers: <EAX>, <EBX>, <ECX>, <EDX>, <ESI>, <EDI>
blur:
    push ebp
    mov ebp, esp

    ; OFFSET
    ; -4 pointer to a new created matrix
    sub esp, 4

    ; Create a new matrix in which we will save the blurred matrix.
    ; The destination matrix will be saved in <EDI>
    mov eax, [img_width]
    imul eax, [img_height]

    push dword 4 ; sizeof(int)
    push eax
    call calloc
    add esp, STACK_WORD * 2

    mov edi, eax
    mov [ebp - 4], eax

    ; Source matrix will be saved in <ESI>
    mov esi, [ebp + 8]

    ; Copy the first line as it is.
    mov ecx, [img_width]
CopySourceFirstLine:
    lodsd
    stosd

    loop CopySourceFirstLine

    ; <ECX> will be the line counter. We also substract -2 because we don't
    ; care about first and last line.
    mov ecx, [img_height]
    sub ecx, 2

LineTraversalSource:
    ; Copy first column as it is
    lodsd
    stosd

    ; <EDX> will be the column counter. We also substract -2 because we don't
    ; care about first and last column.
    mov edx, [img_width]
    sub edx, 2
ColumnTraversalSource:
    mov eax, [esi]

    ; Add right and left element.
    add eax, [esi + 4]
    add eax, [esi - 4]

    push eax

    ; Add lower element.
    mov eax, [img_width]
    imul eax, 4
    add eax, esi
    mov eax, [eax]

    add eax, [esp]
    add esp, STACK_WORD

    push eax

    ; Add upper element.
    mov eax, [img_width]
    imul eax, 4
    neg eax
    add eax, esi
    mov eax, [eax]

    add eax, [esp]
    add esp, STACK_WORD

    ; Divide by 5.
    push edx

    mov ebx, 5
    xor edx, edx
    div ebx

    pop edx

    ; Store the result
    stosd

    ; Go to the next cell in source.
    add esi, 4

    ; Decrement the column counter and continue.
    dec edx
    cmp edx, 0
    jnz ColumnTraversalSource

ColumnTraversalSourceDone:
    ; Copy the last column as it is
    lodsd
    stosd

LineTraversalSourceReload:

    dec ecx
    cmp ecx, 0
    jnz LineTraversalSource

LineTraversalSourceDone:

    ; Copy the last line as it is.
    mov ecx, [img_width]
CopySourceLastLine:
    lodsd
    stosd

    loop CopySourceLastLine

PrintBlurredImage:
    push dword [ebp - 4]
    call PrintImageWrapper
    add esp, STACK_WORD

FreeBlurredMatrix:
    push dword [ebp - 4]
    call free
    add esp, STACK_WORD

    mov esp, ebp
    pop ebp

    ret

    ; Convenience function for printing the image.
    ; #arg1: image to be printed
PrintImageWrapper:
    push ebp
    mov ebp, esp

    push dword [img_height]
    push dword [img_width]
    push dword [ebp + 8]
    call print_image
    add esp, 12

    mov esp, ebp
    pop ebp

    ret
