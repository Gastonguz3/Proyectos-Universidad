template <class T>
Conjunto<T>::Conjunto():_raiz(nullptr), _cardinal(0) {}

template <class T>
Conjunto<T>::~Conjunto() { 
    destruirNodos(_raiz);
}

template <class T>
void Conjunto<T>::destruirNodos(Nodo* actual){
    if(actual != nullptr){
        destruirNodos(actual->izq);
        destruirNodos(actual->der);
        delete actual;
    }
}

template <class T>
bool Conjunto<T>::pertenece(const T& clave) const {
    Nodo* actual = _raiz;
    if(_cardinal == 0){
        return false;
    } else {
        if (_cardinal == 1) {
            if (_raiz->valor == clave) {
                return true;
            } else {
                return false;
            }
        } else {
            while (actual != nullptr) {
                if (actual->valor == clave) {
                    return true;
                } else {
                    if (actual->valor < clave) {
                        actual = actual->der;
                    } else {
                        actual = actual->izq;
                    }
                }
            }
            return false;
        }
    }
}

template <class T>
void Conjunto<T>::insertar(const T& clave) {
    Nodo* anterior = nullptr;
    Nodo* actual = _raiz;
    if(!pertenece(clave)){
        Nodo* nuevo = new Nodo(clave);
        if(_raiz == nullptr){
            _raiz = nuevo;
        } else {
            while(actual != nullptr){
                if(actual->valor < clave){
                    anterior = actual;
                    actual = actual->der;
                } else {
                    anterior = actual;
                    actual = actual->izq;
                }
            }
            if(anterior->valor > clave){
                anterior->izq = nuevo;
            } else{
                anterior->der = nuevo;
            }
        }
        _cardinal++;
    }
}

template <class T>
void Conjunto<T>::remover(const T& clave) {
    Nodo* actual = _raiz;
    Nodo* anterior = nullptr;
    if(pertenece(clave)){
        //caso: si la clave a borrar es la raiz
        if(clave == _raiz->valor) {
            //la raiz no tiene hijos
            if (_raiz->izq == nullptr && _raiz->der == nullptr) {
                _raiz = nullptr;
                delete actual;
                _cardinal--;
                return;
            } else {
                //la raiz tiene un hijo izquierdo
                if (_raiz->der == nullptr) {
                    Nodo* apuntar = _raiz->izq;
                    delete _raiz;
                    _raiz = apuntar;
                    _cardinal--;
                    return;
                } else {
                    //la raiz tiene un hijo derecho
                    if (_raiz->izq == nullptr){
                        Nodo* apuntar = _raiz->der;
                        delete _raiz;
                        _raiz = apuntar;
                        _cardinal--;
                        return;
                    } else {
                        //la raiz tiene 2 hijos
                        Nodo* padre = actual;
                        actual = actual->der;
                        while(actual->izq != nullptr){
                            actual = actual->izq;
                        }
                        T x = actual->valor;
                        remover(actual->valor);
                        padre->valor = x;
                        return;
                    }
                }
            }
        }
        //caso: no es raiz
        //busco el nodo que contiene la clave a borrar
        while(actual->valor != clave){
            if(actual->valor < clave){
                anterior = actual;
                actual = actual->der;
            } else {
                anterior = actual;
                actual = actual->izq;
            }
        }
        //cero hijos
        if(actual->izq == nullptr && actual->der == nullptr){
            if(anterior->valor > actual->valor){
                delete actual;
                anterior->izq = nullptr;
                _cardinal--;
                return;
            } else {
                delete actual;
                anterior->der = nullptr;
                _cardinal--;
                return;
            }
        }
        //un hijo
        else if(actual->der == nullptr){
            if(anterior->valor > actual->valor){
                anterior->izq = actual->izq;
                delete actual;
                _cardinal--;
                return;
            } else{
                anterior->der = actual->izq;
                delete actual;
                _cardinal--;
                return;
            }
        }
        else if(actual->izq == nullptr){
            if(anterior->valor > actual->valor){
                anterior->izq = actual->der;
                delete actual;
                _cardinal--;
                return;
            } else{
                anterior->der = actual->der;
                delete actual;
                _cardinal--;
                return;
            }
        }
        //caso dos hijos
        else{
            Nodo* padre = actual;
            actual = actual->der;
            while(actual->izq != nullptr){
                actual = actual->izq;
            }
            T x = actual->valor;
            remover(actual->valor);
            padre->valor = x;

        }

    }

}

template <class T>
const T& Conjunto<T>::siguiente(const T& clave) {
    Nodo* actual = _raiz;
    Nodo* anterior = _raiz;
    Nodo* sucesor = nullptr;
    if(pertenece(clave)){
        //busco el nodo con la clave
        while(actual->valor != clave){
            if(actual->valor < clave){
                actual = actual->der;
            } else {
                actual = actual->izq;
            }
        }
        //el actual es el nodo con la clave,ahora busco el siguiente
        //caso en el que el nodo tiene subarbol a la derecha
        if(actual->der != nullptr){
            actual = actual->der;
            //de la funcion de minimo
            while(actual->izq != nullptr){
                actual = actual->izq;
            }
            return actual->valor;
        }else{
            //caso en el que el nodo NO tiene subarbol a la derecha
            //el anterior es la raiz principal
            while(actual != anterior){
                if(actual->valor < anterior->valor){
                     sucesor = anterior;
                     anterior = anterior->izq;
                } else {
                    anterior = anterior->der;
                }
            }
            return sucesor->valor;
        }

    }
}

template <class T>
const T& Conjunto<T>::minimo() const {
    Nodo* actual = _raiz;
    while(actual->izq != nullptr){
        actual = actual->izq;
    }
    return actual->valor;
}

template <class T>
const T& Conjunto<T>::maximo() const {
    Nodo* actual = _raiz;
    while(actual->der != nullptr){
        actual = actual->der;
    }
    return actual->valor;
}

template <class T>
unsigned int Conjunto<T>::cardinal() const {
    return _cardinal;
}



