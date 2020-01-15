
/*
Co-Routine implementation
https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
In the context of the C programming language, function arguments are pushed on the stack in the right-to-left order,
i.e. the last argument is pushed first. In Linux, GCC sets the de facto standard for calling conventions. Since GCC version 4.5,
the stack must be aligned to a 16-byte boundary when calling a function (previous versions only required a 4-byte alignment.)

C++ Calling Conventions in ASM

_CDECL

caller:
	; make new call frame
	; (some compilers may produce an 'enter' instruction instead)
	push    ebp       ; save old call frame
	mov     ebp, esp  ; initialize new call frame
	; push call arguments, in reverse
	; (some compilers may subtract the required space from the stack pointer,
	; then write each argument directly, see below.
	; The 'enter' instruction can also do something similar)
	; sub esp, 12      : 'enter' instruction could do this for us
	; mov [ebp-4], 3   : or mov [esp+8], 3
	; mov [ebp-8], 2   : or mov [esp+4], 2
	; mov [ebp-12], 1  : or mov [esp], 1
	push    3
	push    2
	push    1
	call    callee    ; call subroutine 'callee'
	add     eax, 5    ; modify subroutine result
					  ; (eax is the return value of our callee,
					  ; so we don't have to move it into a local variable)
	; restore old call frame
	; (some compilers may produce a 'leave' instruction instead)
	; add   esp, 12   ; remove arguments from frame, ebp - esp = 12.
					  ; compilers will usually produce the following instead,
					  ; which is just as fast, and, unlike the add instruction,
					  ; also works for variable length arguments
					  ; and variable length arrays allocated on the stack.
	mov     esp, ebp  ; most calling conventions dictate ebp be callee-saved,
					  ; i.e. it's preserved after calling the callee.
					  ; it therefore still points to the start of our stack frame.
					  ; we do need to make sure
					  ; callee doesn't modify (or restores) ebp, though,
					  ; so we need to make sure
					  ; it uses a calling convention which does this
	pop     ebp       ; restore old call frame
	ret               ; return

*/







/* 
_DATA SEGMENT
hello_msg db "Hello world", 0
info_msg  db "Info", 0
_DATA ENDS
To define read-only and executable data, do that inside the text segment:

_TEXT    SEGMENT
hello_msg db "Hello world", 0
info_msg  db "Info", 0
_TEXT    ENDS

EXTERN c_ext_byte: byte
EXTERN c_ext_word: word
EXTERN c_ext_dword: dword
EXTERN c_ext_qword: qword
EXTERN c_ext_ptr: qword
EXTERN c_ext_my_function: PROC
EXTERN MessageBoxA: PROC
EXTERN GetForegroundWindow: PROC

PUBLIC access_extern_data
access_extern_data PROC
; Dereference all the data according to each data's sizes
mov    al, byte ptr [c_ext_byte]
mov ax, word ptr [c_ext_word]
mov eax, dword ptr [c_ext_dword]
mov rax, qword ptr [c_ext_qword]
; Remember, a pointer is just a QWORD
mov rax, qword ptr [c_ext_ptr]
; Similarily, a function pointer is also a QWORD
mov rax, offset c_ext_my_function
sub rsp, 4 * 8 ; Register shadow stack
call rax ; call the C function
add rsp, 4 * 8 ; Restore the stack
ret
access_extern_data ENDP

EXTERN MessageBoxA: PROC
EXTERN GetForegroundWindow: PROC
PUBLIC hello_world_asm
hello_world_asm PROC
push rbp ; save frame pointer
mov rbp, rsp ; fix stack pointer
sub rsp, 8 * (4 + 2) ; allocate shadow register area + 2 QWORDs for stack alignment
; Get a window handle
call GetForegroundWindow
mov rcx, rax
; WINUSERAPI int WINAPI MessageBoxA(
;  RCX =>  _In_opt_ HWND hWnd,
;  RDX =>  _In_opt_ LPCSTR lpText,
;  R8  =>  _In_opt_ LPCSTR lpCaption,
;  R9  =>  _In_ UINT uType);
mov rdx, offset hello_msg
mov r8, offset info_msg
mov r9, 0 ; MB_OK
and rsp, not 8 ; align stack to 16 bytes prior to API call
call MessageBoxA
; epilog. restore stack pointer
mov rsp, rbp
pop rbp
ret
hello_world_asm ENDP

*/ 