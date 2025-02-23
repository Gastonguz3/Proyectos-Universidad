#include "Lista.h"
#include <cassert>
Lista::Lista() : _prim(nullptr), _ult(nullptr),  _size(0) {}

Lista::Lista(const Lista& l) : Lista() {
    //Inicializa una lista vacía y luego utiliza operator= para no duplicar el código de la copia de una lista.
    copiarnodo(l);
}

Lista::~Lista() {
    while(_prim != nullptr){
        Nodo* siguiente = _prim->atras;
        delete _prim;
        _prim = siguiente;
    }
}

void Lista::destruirnodo(){
    Nodo* actual= _prim;
    while(actual != nullptr){
        Nodo* siguiente = actual->atras;
        delete actual;
        actual = siguiente;
    }
    _prim = nullptr;
    _ult = nullptr;
    _size = 0;
}

void Lista::copiarnodo(const Lista &l){
    Nodo* actual = l._prim;
    if(l._size == 0){
        return;
    }else{
        while(actual != l._ult){
            agregarAtras(actual->valor);
            actual = actual->atras;
        }
        agregarAtras(actual->valor);
    }
}

Lista& Lista::operator=(const Lista& aCopiar) {
    //destruir nodos
    destruirnodo();
    //copiar nodos
    copiarnodo(aCopiar);
    return *this;
}

void Lista::agregarAdelante(const int& elem) {
    Nodo* nuevo = new Nodo(nullptr,elem, nullptr);
    if(_prim == nullptr){
        _prim = nuevo;
    } else{
        if(_size == 1){
            nuevo->atras = _prim;
            _ult = nuevo->atras;
            _prim = nuevo;
        }else{
            _prim->adel = nuevo;
            nuevo->atras = _prim;
            _prim = nuevo;
        }

    }
    _size++;

}

void Lista::agregarAtras(const int& elem) {
    Nodo* nuevo = new Nodo(nullptr, elem, nullptr);
    if(_prim == nullptr){
        _prim = nuevo;
    }else{
        if(_size == 1){
            nuevo->adel = _prim;
            _ult = nuevo;
            _prim->atras = _ult;
        } else {
            _ult->atras = nuevo;
            nuevo->adel = _ult;
            _ult = nuevo;
        }
    }
    _size++;

}

void Lista::eliminar(Nat i) {
    Nodo* actual = _prim;
    for(int j = 0; j < i; j++){
        actual = actual->atras;
    }
    if(_size == 1){
        _prim = nullptr;
        delete actual;
    }
    //caso primer elem eliminado
    else if(actual->adel == nullptr){
            _prim = actual->atras;
            _prim->adel = nullptr;
            delete actual;
    }
    //caso ultimo elem eliminado
    else if(actual->atras == nullptr){
        _ult = actual->adel;
        _ult->atras = nullptr;
        delete actual;
    }else{
        //caso medio eliminado
        actual->adel->atras = actual->atras;
        actual->atras->adel = actual->adel;
        delete actual;
    }
    _size--;
}

int Lista::longitud() const {
    return _size;
}

const int& Lista::iesimo(Nat i) const {
    Nodo* actual = _prim;
    for(int j = 0; j < i; j++){
            actual = actual->atras;
    }
    return actual->valor;
}

int& Lista::iesimo(Nat i) {
    Nodo* actual = _prim;
    for(int j = 0; j < i; j++){
            actual = actual->atras;
    }
    return actual->valor;
}

