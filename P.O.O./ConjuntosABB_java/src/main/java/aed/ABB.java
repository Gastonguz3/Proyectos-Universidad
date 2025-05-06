package aed;

import java.util.*;

// Todos los tipos de datos "Comparables" tienen el método compareTo()
// elem1.compareTo(elem2) devuelve un entero. Si es mayor a 0, entonces elem1 > elem2
public class ABB<T extends Comparable<T>> implements Conjunto<T> {
    private Nodo raiz;
    private int cantNodos;

    private class Nodo {
        T valor;
        Nodo izq;
        Nodo der;
        Nodo padre;
        Nodo(T v){
            valor = v;
            izq = null;
            der = null;
            padre = null;
        }
    }

    public ABB() {
        raiz = null;
        cantNodos = 0;
    }

    public int cardinal() {
        return cantNodos;
    }

    public T minimo(){
        Nodo actual = raiz;
        while(actual.izq != null){
            actual = actual.izq;
        }
        return actual.valor;

    }

    public T maximo(){
        Nodo actual = raiz;
        while(actual.der != null){
            actual = actual.der;
        }
        return actual.valor;
    }

    public void insertar(T elem){
        if(raiz == null){
            Nodo nuevo = new Nodo(elem);
            raiz = nuevo;
            cantNodos++;
            return;
        } else {
           if(!pertenece(elem)){
                Nodo actual = raiz;
                Nodo anterior = actual.padre;
                while(actual != null){
                    if(actual.valor.compareTo(elem) > 0){
                        //el actual es mayor q el elemento ->voy a la izq porque quiero uno menor
                        anterior = actual;
                        actual = actual.izq;
                    } else {
                        //el actual es menor q el elemento ->voy a la der porque quiero uno mayor
                        anterior = actual;
                        actual = actual.der;
                    } 
                }
                // actual es el nodo donde debo insertar el nuevo, me ayudo del anterior
                Nodo nuevo = new Nodo(elem);
                if(anterior.valor.compareTo(elem) > 0){
                    anterior.izq = nuevo;
                    nuevo.padre = anterior;
                } else {
                    anterior.der = nuevo;
                    nuevo.padre = anterior;
                }
                cantNodos++;
                return;
            }
            return;
        }

    
    }

    public boolean pertenece(T elem){
        if(raiz != null || cantNodos != 0){
            Nodo actual = raiz;
            if(cantNodos == 1){
                if(raiz.valor.compareTo(elem) == 0){
                    return true; 
                } else {
                    return false;
                }
            } else {
                while(actual != null){
                    if(actual.valor.compareTo(elem) > 0){
                        //el actual es mayor q el elemento buscado->voy a la izq porque quiero uno menor
                        actual = actual.izq;
                    } else {
                        if(actual.valor.compareTo(elem) < 0){
                            //el actual es menor q el elemento buscado->voy a la der porque quiero uno mayor
                            actual = actual.der;
                        } else {
                            //son iguales-> esta
                            return true;
                        }
                    }
                }
            //no se encontro el elem -> false
            return false;
            }
            
        } else {
            return false;
        }
    }

    public void eliminar(T elem){
        if(pertenece(elem)){
            Nodo actual = raiz;
            Nodo anterior = null;

            //caso es raiz
            if(raiz.valor.compareTo(elem) == 0){
                if (raiz.izq == null && raiz.der == null) {
                    raiz = null;
                    cantNodos--;
                    return;
                } else {
                    if (raiz.der == null) {
                        Nodo NuevaRaiz = raiz.izq;
                        raiz = NuevaRaiz;
                        cantNodos--;
                        return;
                    } else {
                        if (raiz.izq == null){
                            Nodo NuevaRaiz = raiz.der;
                            raiz = NuevaRaiz;
                            cantNodos--;
                            return;
                        } else {
                            Nodo padrePrincipal = actual;
                            actual = actual.der;
                            while(actual.izq != null){
                                actual = actual.izq;
                            }
                            T numeroAcopiar = actual.valor;
                            eliminar(actual.valor);
                            padrePrincipal.valor = numeroAcopiar;
                            return;
                        }
                    }
                }    
            }
            //caso no es raiz
            while(actual.valor.compareTo(elem) != 0){
                if(actual.valor.compareTo(elem) > 0){
                    //el actual es mayor q el elemento ->voy a la izq porque quiero uno menor
                    anterior = actual;
                    actual = actual.izq;
                } else {
                    //el actual es menor q el elemento ->voy a la der porque quiero uno mayor
                    anterior = actual;
                    actual = actual.der;
                } 
            }
            //actual es el nodo q quiero eliminar
            //anterior es el padre del nodo a eliminar
            if(actual.izq == null && actual.der == null){
                //caso q no tiene hijos (es hoja)
                if(anterior.valor.compareTo(actual.valor) > 0){
                    anterior.izq = null;
                } else {
                    anterior.der = null;
                }
            } else {
                if(actual.izq != null && actual.der == null){
                    //caso 1 hijo: hijo a la izq
                    if(anterior.valor.compareTo(actual.valor) > 0){
                            anterior.izq = actual.izq;
                            // actual.izq = null;
                    } else {
                            anterior.der = actual.izq;
                            // actual.izq = null; 
                    } 

                } else {
                    if(actual.izq == null && actual.der != null){
                        //caso 1 hijo: hijo a la der
                        if(anterior.valor.compareTo(actual.valor) > 0){
                            anterior.izq = actual.der;
                            // actual.der = null;
                            
                        } else {
                            anterior.der = actual.der;
                            // actual.der = null;
                            
                        }               

                    } else {
                        //caso 2 hijos
                        Nodo padrePrincipal = actual;
                        //busco sucesor inmediato
                        actual = actual.der;
                        while(actual.izq != null){
                            actual = actual.izq;
                        }
                        T numeroAcopiar = actual.valor;
                        cantNodos++; //cuando entra al eliminar resta dos veces la cant de nodos, para q no pase eso le sumo uno para equilibrar 
                        eliminar(actual.valor);
                        //copio el valor del sucesor al valor q quiero eliminar
                        padrePrincipal.valor = numeroAcopiar;

                    }
                }
            }
            cantNodos--;
            return;
        } 

        return;
    }

    //Devuelve un String con los elementos del conjunto en ORDEN, escritos entre llaves y separados por coma (e.g., {1, 3, 4, 6, 10})
    //hay q hacer el recorrido inorder
    public String toString(){
        StringBuffer sbuffer = new StringBuffer();
        Nodo actual = raiz;
        sbuffer.append("{");

        Nodo primero = raiz;
        while(primero.izq != null){  // coloco el primero elemento
            primero = primero.izq;
        }

        sbuffer.append(primero.valor);

        while(actual != null){
            sbuffer.append(toString2(actual, primero));  

            if(actual.der != null){
                sbuffer.append(",");
                sbuffer.append(actual.valor);
            }

            actual = actual.der;

        }

        sbuffer.append("}");
        return sbuffer.toString();
    }

    //nueva funcion
    public String toString2(Nodo actual, Nodo primero){
        StringBuffer sbuffer = new StringBuffer();
        Nodo anterior = null;
        int cont = 0;   //para que no me tome como anterior al nodo "raiz" de los subarboles
        while(actual.izq != null){
            anterior = actual;
            actual = actual.izq;
            cont++;

        }

        if(actual != primero){
            sbuffer.append(",");
            sbuffer.append(actual.valor);
        }
        
        if(actual.der != null){
            sbuffer.append(toString2(actual.der, primero));
        }

        if(anterior != null && cont > 1){
            sbuffer.append(",");
            sbuffer.append(anterior.valor);

            if(anterior.der != null){
                sbuffer.append(toString2(anterior.der, primero));
            }
            
        }


        return sbuffer.toString();

    }

    Nodo minimo2(Nodo actual){
        while(actual.izq != null){
            actual = actual.izq;
        }
        return actual;
    }

    private class ABB_Iterador implements Iterador<T> {
        private Nodo _actual;   
        private int it;
        ABB_Iterador(){
            it = 0;
            _actual = minimo2(raiz); //arranco por el menor valor
        }
        

        public boolean haySiguiente() {   
            Nodo anterior = _actual.padre;
            if(anterior != null){
                return true;
            } else {
                return false;
            }
        }
    
        public T siguiente() {
            Nodo anterior = _actual.padre;
            Nodo padrePrincipal = raiz;

            Nodo ViejoActual = _actual; //valor a devolver

            if(_actual.der != null){
                //devuelvo el minimo del subarbol derecho
                _actual = minimo2(_actual.der);         

            } else {
                if(anterior != null && anterior.valor.compareTo(_actual.valor) > 0){
                    //si el anterior esta del lado derecho del nodo actual continuo por ahi
                    _actual = anterior;
                } else {
                    if(anterior == raiz){
                        //si retrocedo y llego a la raiz
                        raiz = _actual.der;
                        _actual = minimo2(_actual.der);
                    } else {
                        //si estoy en la parte baja del arbol y ya no puedo continuar regreso para arriba 
                        _actual = padrePrincipal.der;
                        raiz = padrePrincipal.der;
                    }
                }
            }

            it++;
            return ViejoActual.valor;

        }
    }

    public Iterador<T> iterador() {
        return new ABB_Iterador();
    }

}
