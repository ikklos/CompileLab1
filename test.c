int calc(int a){
    return a*2;
}
int main()
{
    int a = 1;
    if(a > 0){
        println_int(1);
    }
    println_int(calc(a)-a);
    return 0;
}