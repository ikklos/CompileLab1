.intel_syntax noprefix
.data
format_str:
.asciz "%d\n"
.extern printf
.text
.global main
calc:
push ebp
mov ebp,esp
sub esp,4
mov eax,DWORD PTR[ebp+8]
push eax
push 2
pop ebx
pop eax
mul ebx
push eax
pop eax
add esp,4
ret
main:
push ebp
mov ebp,esp
sub esp,8
push 1
pop eax
mov [ebp-4],eax
mov eax,DWORD PTR[ebp-4]
push eax
push 0
pop ebx
pop eax
mov ecx,1
cmp eax,ebx
jg _T2
dec ecx
_T2:
push ecx
pop eax
test eax
je _T0
push 1
push offset format_str
call printf
add esp,8
jmp _T1
_T0:
_T1:
mov eax,DWORD PTR[ebp-4]
push eax
call calc
add esp,4
push eax
mov eax,DWORD PTR[ebp-4]
push eax
pop ebx
pop eax
sub eax,ebx
push eax
push offset format_str
call printf
add esp,8
push 0
pop eax
add esp,8
ret
