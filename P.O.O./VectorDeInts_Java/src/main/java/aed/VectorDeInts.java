package aed;

class VectorDeInts implements SecuenciaDeInts {
    private static final int CAPACIDAD_INICIAL = 1;
    private int size;
    private int[] array;

    public VectorDeInts() {
        size = 0;
        array = new int[1];
    }

    //constructor por copia
    public VectorDeInts(VectorDeInts vector) {
        size = vector.size;
        array = new int[vector.array.length];
        for(int i = 0; i < vector.array.length; i++) {
            array[i] = vector.array[i];
        }
    }

    public int longitud() {
        return size;
    }

    public void agregarAtras(int i) {
        //agrega a la izquierda [1,2,3] -> [0,1,2,3];  v[4] = 0
        //caso base
        if(size == 0) {
            array[0] = i;
        } else  {
            if(size == array.length){
                int[] NuevoArray = new int[size*2];
                for(int k = 0; k < size; k++){
                    NuevoArray[k] = array[k];
                }
                NuevoArray[size] = i;
                array = NuevoArray;
            } else {
                array[size] = i;
            }
        }
        size++;
    }

    public int obtener(int i) {
        return array [i];
    }

    public void quitarAtras() {
        int[] NuevoArray2 = new int[size - 1];
        for(int i = 0; i < size - 1; i++){
            NuevoArray2[i] = array[i];
        }
        size--;
    }

    public void modificarPosicion(int indice, int valor) {
        array[indice] = valor;
    }

    public VectorDeInts copiar() {
        int[] NuevoVector = new int[size];
        for(int i = 0; i < size; i++){
            NuevoVector[i] = array[i];
        }
        VectorDeInts res = new VectorDeInts();
        res.size = NuevoVector.length;
        res.array = NuevoVector;
        return res;
   
    }

}
