package aed;

import java.util.*;

public class ListaEnlazada<T> implements Secuencia<T> {
    private Nodo primero;
    private Nodo ultimo;
    private int size; 

    private class Nodo {
        T valor;
        Nodo sig;
        Nodo ant;
        Nodo(T v){valor = v; sig = null; ant = null;}
    }

    public ListaEnlazada() {
        primero = null;
        ultimo = null;
        size = 0;
    }

    public int longitud() {
        return size;
    }

    public void agregarAdelante(T elem) {
         //agrega al principio [size, ]-> [size, , , , ]
        Nodo nuevo = new Nodo(elem);
        if(size == 0){
            primero = nuevo;
        } else{
            if(size == 1){
                //el nuevo nodo pasa a ser el primero y el nodo que esta pasa a ser el ultimo
                nuevo.sig = primero;
                ultimo = nuevo.sig;
                primero = nuevo;
            } else {
                primero.ant = nuevo;
                nuevo.sig = primero;
                primero = nuevo;
            }
        }
        size++;
    }

    public void agregarAtras(T elem) {
        Nodo nuevo = new Nodo(elem);
        if(size == 0) {
            //caso base
            primero  = nuevo;
            primero.sig = null;
            primero.ant = null;            
        } else {
            if(size == 1){
                //no existe un "ultimo" en este caso, el nuevo nodo sera ese ultimo
                nuevo.ant = primero;
                ultimo = nuevo;
                primero.sig = ultimo;
            } else {
                //existe un ultimo y nos ayudamos para colocar el nuevo, ese nuevo sera luego el ultimo
                ultimo.sig = nuevo;
                nuevo.ant = ultimo;
                ultimo = nuevo;
            }

        }
        size++;
    
    }

    public T obtener(int i) {
        Nodo actual = primero;
        for(int j = 0; j < i; j++){
            actual = actual.sig;
        }
        return actual.valor;

    }

    public void eliminar(int i) {

        Nodo actual = primero;
        for(int j = 0; j < i; j++){
            actual = actual.sig;
        }
        //actual es el nodo q quiero eliminar
        if(size == 1){
            //caso: un solo elemento
            primero = null;
        } else {
            if(actual.ant == null){
            //caso: se elimina el primer elem
            //aca no entro en el for asi q actual = primero && prev = primero
            primero = actual.sig;
            primero.ant = null;
            } else {
                if(actual.sig == null){
                    //caso: se elimina el ultimo elemento
                   ultimo = actual.ant;
                   ultimo.sig = null;

                } else {
                    //caso: se elimina un elemento del medio
                   actual.sig.ant = actual.ant;
                   actual.ant.sig = actual.sig;
                }
           }
        }
        
        size--;
    }

    public void modificarPosicion(int indice, T elem) {
        Nodo actual = primero;
        for(int j = 0; j < indice; j++){
            actual = actual.sig;
        }
        actual.valor = elem;

    }

    public ListaEnlazada<T> copiar() {
        ListaEnlazada<T> nuevaLista = new ListaEnlazada<T>();
        int newSize = 0;
        Nodo actual = primero;
        while(actual != null){
            nuevaLista.agregarAtras(actual.valor);
            newSize++;
            actual = actual.sig;
        }
        nuevaLista.size = newSize;
        return nuevaLista;
    }

    public ListaEnlazada(ListaEnlazada<T> lista) {
        ListaEnlazada<T> nuevaLista = lista.copiar();
        primero = nuevaLista.primero;
        ultimo = nuevaLista.ultimo;
        size = nuevaLista.size;
    }
    
    @Override
    public String toString() {
        StringBuffer sbuffer = new StringBuffer();
        sbuffer.append("[");
        sbuffer.append(primero.valor);
        while(primero.sig != null){
            sbuffer.append(", ");
            sbuffer.append(primero.sig.valor);
            primero = primero.sig;
        }
        sbuffer.append("]");
        return sbuffer.toString();

    }

    private class ListaIterador implements Iterador<T> {
        int it;
        ListaIterador(){ it = 0;  }

        public boolean haySiguiente() {
            return it < size;
        }
        
        public boolean hayAnterior() {
	        return it > 0;
        }

        public T siguiente() {
	        int i = it;
            it++;
            return obtener(i);
        }
        

        public T anterior() {
            it--;
            return obtener(it);
        }
    }

    public Iterador<T> iterador() {
	    return new ListaIterador();
    }

}