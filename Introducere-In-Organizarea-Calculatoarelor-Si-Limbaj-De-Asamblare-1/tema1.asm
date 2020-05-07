%include "includes/io.inc"

extern getAST
extern freeAST

section .data
    Operator db "+-*/", 0x00
    OperatorLength EQU $ - Operator - 1

    NULL equ 0x00
    TEN  equ 0x0a
    STACKWORD equ 0x04

section .bss
	; La aceasta adresa, scheletul stocheaza radacina arborelui
	root: resd 1

section .text
    global main

main:
	; NU MODIFICATI
	push ebp
	mov ebp, esp

	; Se citeste arborele si se scrie la adresa indicata mai sus
	call getAST
	mov [root], eax

    push eax
    call EvaluateAST
    add esp, 4

    ; print the result to stdout
    PRINT_DEC 4, eax
    NEWLINE

	; NU MODIFICATI
	; Se elibereaza memoria alocata pentru arbore
	push dword [root]
	call freeAST

	xor eax, eax
	leave
	ret


;#######################################################################
    ; int CheckIfSubtreeIsOperatorSubroutine(char *string)
    ; return value : 1 if the subtree content is an operator
    ;               0 is the subtree is numeric value
    ; error value  : prints error message and exits

    ; Description :
    ; When the execution is passed to this subroutine, it checks whether
    ; the string stored in <ESI> is 1 byte long. If it is greater than 1
    ; byte it means that the subtree content is not an operator. On the other
    ; hand if the value is 1 byte long and it lands in the range 0x2a - 0x2f
    ; it means that the subtree content is an operator.

    ; Modified registers: <ESI>, <EAX>
CheckIfSubtreeIsOperatorSubroutine:
    push ebp
    mov ebp, esp

    mov esi, [ebp + 8]

    ; check if content of the subtree is a numeric value or
    ; an operator by computing the length of the string. if the
    ; length is greater than 1 the subtree is clearly a number.
    ; if the length is 1 it may be a 1 digit number.
    push esi
    call StringLengthSubroutine
    pop esi

    cmp eax, 0x01
    jnz SubtreeIsNumericValue

    ; ascii for * + - / are 0x2a 0x2b 0x2d and 0x2f
    ; it means that if the byte in esi is between 0x2a and 0x2f
    ; the subtree content is an operator
FurtherCheck:
    lodsb

    ; substract 0x2a from 0x2a and 0x2a from 0x2f
    ; if al - 0x2a lands between 0x00 and 0x05 it means that the
    ; byte is an operator
    sub al, 0x2a

    cmp al, 0x05
    ja SubtreeIsNumericValue

SubtreeIsOperator:
    mov eax, 1
    jmp SubroutineReturn

SubtreeIsNumericValue:
    xor eax, eax

SubroutineReturn:
    mov esp, ebp
    pop ebp

    ret

;################ End of CheckIfSubtreeIsOperatorSubroutine ############


;#######################################################################
    ; int EvaluateAST(struct_ast *root)
    ; return value : the integer computed after each operation from the
    ;   AST was successfully processed

    ; Description :
    ; The subroutine will go recursivelly down the tree and will check
    ; whether the current subtree content is an operator or is a
    ; numerical value. If the subtree content is numeric value then
    ; return it, on the other hand if subtree content is operand then
    ; compute 'leftsubtree `operand` rightsubtree'.

    ; Modified registers : <EAX>, <ECX>, <EDX>, <ESI>
EvaluateAST:
    push ebp
    mov ebp, esp

    sub esp, 16
    ; OFFSET EXPLANATION
    ; -4  current struct pointer
    ; -8  current operator
    ; -12 leftsubtree numeric value
    ; -16 rightsubtree numeric value

    ; save struct pointer in a local variable
    mov edx, [ebp + 8]
    mov [ebp - 4], edx

    mov edx, [edx]

    push edx
    call CheckIfSubtreeIsOperatorSubroutine
    pop edx

    ; if the subtree content is an operator then make operation
    ; between left subtree and right subtree
    cmp eax, 1
    jnz ReturnNumericValue

ReturnOperationResult:
    ; save operator because registers will change after the recursive call
    mov edx, [edx]
    mov [ebp - 8], edx

    ; <---- go to left subtree ---->
    mov edx, [ebp - 4]
    add edx, 4
    mov edx, [edx]

    push edx
    call EvaluateAST
    add esp, 4

    ; store result
    mov [ebp - 12], eax

    ; <---- go to right subtree ---->
    mov edx, [ebp - 4]
    add edx, 8
    mov edx, [edx]

    push edx
    call EvaluateAST
    add esp, 4

    ; store result
    mov [ebp - 16], eax

    ; <---- end recursion, start operation ---->
    mov eax, [ebp - 12]
    mov ecx, [ebp - 16]

    mov edx, [ebp - 8]

    ; <---- Multiplication ---->
    cmp dl, 0x2a ; ascii for *
    jnz SkipMultiplicationOperation

MultiplicationOperation:
    xor edx, edx
    imul eax, ecx
    jmp OperationDone

SkipMultiplicationOperation:

    ; <---- Addition ---->
    cmp dl, 0x2b ; ascii for +
    jnz SkipAdditionOperation

AdditionOperation:
    add eax, ecx
    jmp OperationDone

SkipAdditionOperation:

    ; <---- Substraction ---->
    cmp dl, 0x2d ; ascii for -
    jnz SkipSubstractionOperation

SubstractionOperation:
    sub eax, ecx
    jmp OperationDone

SkipSubstractionOperation:

    ; <---- Division ---->
    cmp dl, 0x2f ; ascii for /
    jnz SkipDivisionOperation

DivisionOperation:
    cdq
    idiv ecx
    jmp OperationDone

SkipDivisionOperation:

OperationDone:
    jmp ReturnFromSubroutine

    ; This is the label the subroutine jumps to when the
    ; content is a number that needs to be converted
ReturnNumericValue:
    push edx
    call StringToNumberSubroutine
    add esp, 4

ReturnFromSubroutine:
    mov esp, ebp
    pop ebp

    ret
;##################### End of EvaluateAST ##############################


; This should be another file but the vmchecker allows only one file to
; be uploaded. In the README the contents below will be refered as
; strtonum.asm

;#######################################################################

    ; int StringToNumberSubroutine(char *string);
    ; return value: <EAX> an integer representing the number from string

    ; Description:
    ; First and foremost compute the length of the string using
    ; the StringLengthSubroutine. Afterwards add the digits to
    ; the returned 4 byte number using the following formula, i.e.
    ; number = number*10 + (digit - '0'). Digit must be converted
    ; from character to number. The value is returned in <EAX>

    ; Modified registers: <EAX>, <ECX>, <ESP>
StringToNumberSubroutine:
    push ebp
    mov ebp, esp

    sub esp, 4 ; store boolean. It is set if number contains minus sign

    ; pass the argument
    mov esi, [ebp + 8]

    ; check if the number contains a minus sign
    call CheckForMinusSignSubroutine
    mov [ebp - 4], eax

    push esi
    call StringLengthSubroutine
    pop esi

    mov ecx, eax ; use <ECX> as counter for the string in <ESI>
    xor eax, eax ; <EAX> will be used as accumulator for number

ComputeNumber:
    ; number = number*10 + (digitAsChar - '0')
    imul eax, TEN

    push eax

    ; store digit in <AL> and add it to number
    xor eax, eax
    lodsb
    sub al, '0'

    add eax, [esp]
    add esp, STACKWORD

    dec ecx
    cmp ecx, 0
    jnz ComputeNumber

ComputeNumberDone:

AddMinusSignToFinalNumber:
    pop ecx

    cmp ecx, 0
    jz StringToNumberDone

NegateNumber:
    neg eax

StringToNumberDone:

    mov esp, ebp
    pop ebp

    ret

;################# End of StringToNumberSubroutine #####################

;#######################################################################

    ; int StringLengthSubroutine(char *string)
    ; return value: <EAX> an integer representing the length of string

    ; Description:
    ; Load a the string in <ESI> and increment <ECX> until
    ; <ESI> is NULL

    ; Modified registers: <EAX>, <ECX>, <ESI>

StringLengthSubroutine:
    push ebp
    mov ebp, esp

    mov esi, [ebp + 8]

    xor eax, eax
    xor ecx, ecx

ComputeLength:
    lodsb ; load a byte from <ESI> to <AL>
    cmp al, NULL
    jz ComputeLengthDone

    inc ecx
    jmp ComputeLength
ComputeLengthDone:

    ; return <EAX> and clear <ECX>
    mov eax, ecx
    xor ecx, ecx

    mov esp, ebp
    pop ebp

    ret
;################# End of StringLengthSubroutine #####################

;#######################################################################

    ; int CheckForMinusSignSubroutine(char *string)
    ; return value: 1 if string contains '-' else 0

    ; Description;
    ; Check first byte of esi and compare it with the ascii value of '-'.
    ; If the number contains a minus sign then delete it because
    ; the StringToNumberSubroutine's ComputeNumber and StringLengthSubroutine
    ; will not take it in account
    ; and this might destroy the number.
    ; Return value is stored in <EAX>

    ; Modified registers: <EAX>

CheckForMinusSignSubroutine:
    xor eax, eax

    ; take the first byte from <ESI>
    mov al, [esi]
    cmp al, '-'

    jz ReturnContainsMinus
ReturnDoesNotContainMinus:
    xor eax, eax
    ret

ReturnContainsMinus:
    inc esi
    mov eax, 1
    ret
;################# End of CheckForMinusSignSubroutine  #####################
