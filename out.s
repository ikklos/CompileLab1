.intel_syntax noprefix
.global main
.extern printf
.align 4
.data
format_str:
.asciz "%d\n"
.text
main:
push ebp
mov ebp,esp
sub esp,8
push 0
pop eax
mov [ebp-4],eax
_T0:
mov eax,DWORD PTR[ebp-4]
push eax
push 3
pop ebx
pop eax
mov ecx,1
cmp eax,ebx
jl _T2
dec ecx
_T2:
push ecx
pop eax
cmp eax,0
je _T1
mov eax,DWORD PTR[ebp-4]
push eax
push offset format_str
call printf
add esp,8
mov eax,DWORD PTR[ebp-4]
push eax
push 1
pop ebx
pop eax
add eax,ebx
push eax
pop eax
mov [ebp-4],eax
jmp _T0
_T1:
push 0
pop eax
leave
ret
