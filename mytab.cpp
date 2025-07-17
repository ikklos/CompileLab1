#include"mytab.h"
int MyTab::getLayer(){
    return this->layer;
}
void MyTab::in(){
    this->layer += 1;
    this->localcounts.push(0);
}
int MyTab::out(){
    int res = 0;
    for(int i = arr.size()-1; i >= 0; i--){
        if(arr[i].layer >= layer){
            int id = mp[arr[i].name];
            mp[arr[i].name] = 0;
            for(int j = id-2; j >= 0; j--){
                if(arr[j].name == arr[i].name){
                    mp[arr[i].name] = j+1;
                }
            }
            res++;
            arr.pop_back();
        }
    }
    layer--;
    localcounts.pop();
    return res;
}
void MyTab::reg(std::string str, std::string type, int argindex){
    int id = mp[str];
    int p = 0;
    if(type == "local"){
        p = ++localcounts.top();
    }else if(type == "arg"){
        p = argindex;
    }
    SInfo info = {
        str,
        type,
        p,
        layer
    };
    arr.push_back(info);
    mp[str] = arr.size();
}

SInfo MyTab::acc(std::string str){
    int id = mp[str];
    if(id <= 0 || id > arr.size()){
        throw std::runtime_error("Info not found");
    }
    return arr[id-1];
}