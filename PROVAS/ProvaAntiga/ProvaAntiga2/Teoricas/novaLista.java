class CelulaDupla {
    int elemento;
    CelulaDupla prox, ant;

    CelulaDupla() {
    }

    CelulaDupla(int elemento) {
        this.elemento = elemento;
    }

}

public class novaLista {
    private static CelulaDupla getTail(CelulaDupla head) {
        CelulaDupla curr = head;
        while (curr != null && curr.prox != null) {
            curr = curr.prox;
        }
        return curr;
    }

    private static CelulaDupla prependHead(CelulaDupla head, int elemento) {
        CelulaDupla novo = new CelulaDupla(elemento);
        if (head == null) {
            return novo;
        } else {
            head.ant = novo;
            novo.prox = head;
            return novo;
        }
    }

    public static CelulaDupla intercalarReverso(CelulaDupla c1, CelulaDupla c2) {
        CelulaDupla t1 = getTail(c1);
        CelulaDupla t2 = getTail(c2);

        CelulaDupla headRes = null;
        boolean vezLista1 = true;

        while (t1 != null || t2 != null) {
            if (vezLista1) {
                if (t1 != null) {
                    headRes = prependHead(headRes, t1.elemento);
                    t1 = t1.ant;
                } else if (t2 != null) {
                    headRes = prependHead(headRes, t2.elemento);
                    t2 = t2.ant;
                }
            } else {
                if (t2 != null) {
                    headRes = prependHead(headRes, t2.elemento);
                    t2 = t2.ant;
                } else if (t1 != null) {
                    headRes = prependHead(headRes, t1.elemento);
                    t1 = t1.ant;
                }
            }
        }
        vezLista1 = !vezLista1;
        return headRes;
    }
}
