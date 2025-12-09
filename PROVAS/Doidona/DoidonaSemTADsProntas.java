class DoidonaSemTADsProntas {
   final int TAMT1 = 100;
   final int TAMT3 = 100;
   final int NULO = -0x7FFFFF;

   int[] t1;
   int[] t3;

   Celula primeiroListaT2, ultimoListaT2;
   No raizArvoreT2, raizArvoreT3;

   public Doidona (){
      t1 = new int[TAMT1];
      t3 = new int[TAMT3];

      for(int i = 0; i < TAMT1; i++){
         t1[i] = NULO;
      }
      for(int i = 0; i < TAMT3; i++){
         t3[i] = NULO;
      }

      primeiroListaT2 = ultimoListaT2 = new Celula();

      raizArvoreT2 = raizArvoreT3 = null;
   }

   public int hashT1(int elemento) {
      // alguma logica de hash
   }

   public int hashT2(int elemento) {
      return elemento % 3; // se nao funcionar no hashT1, aqui ele vera para qual das outras 3 estuturas ir
   }

   public int hashT3(int elemento) {
      // ira fazer outro hash para ver onde inserir na tabela T3
   }

   public int rehashT3(int elemento) {
      // Caso haja colisao na T3, ira fazer um rehash para tentar achar outro lugar
   }

   public void inserir(int elemento) {
      int i = hashT1(elemento);

      if (elemento == NULO) {
         // gerar msg de erro!!!!
      } else if (t1[i] == NULO) {
         t1[i] = elemento;
      } else if (hashT2(elemento) == 0) {
         inserirT3(elemento);
      } else if (hashT2(elemento) == 1) {
         inserirLista(elemento);
      } else if (hashT2(elemento) == 2) {
         raizArvoreT2 = inserirArvore(raizArvoreT2, elemento);
      } else {
         System.out.println("ERRO!!!!");
      }
   }

   public void inserirT3(int elemento) {
      int i = hashT3(elemento);

      if (t3[i] == NULO) {
         t3[i] = elemento;
      } else {
         i = rehashT3(elemento);

         if (t3[i] == NULO) {
            t3[i] = elemento;
         } else {
            raizArvoreT3 = inserirArvore(raizArvoreT3, elemento);
         }
      }
   }

   public void inserirLista(int elemento) {
      ultimoListaT2.prox = new Celula(elemento);
      ultimoListaT2 = ultimoListaT2.prox;
   }

   public No inserirArvore(No no, int elemento) {
      if (no == null) {
         no = new No(elemento);
      } else if (no.elemento > elemento) {
         no.esq = inserirArvore(no.esq, elemento);
      } else if (no.elemento < elemento) {
         no.dir = inserirArvore(no.dir, elemento);
      } else {
         System.out.println("Erro de Insercao");
      }
      return no;
   }

   public boolean pesquisar(int elemento) {
      boolean resp;
      int i = hashT1(elemento);
      if (t1[i] == NULO) {
         resp = false;
      } else if (t1[i] == elemento) {
         resp = true;
      } else if (hashT2(elemento) == 0) {
         resp = pesquisarT3(elemento);
      } else if (hashT2(elemento) == 1) {
         resp = pesquisarLista(elemento);
      } else if (hashT2(elemento) == 2) {
         resp = pesquisarArvore(raizArvoreT2, elemento);
      } else {
         resp = false;
         System.out.println("ERRO!!!!");
      }
      return resp;
   }

   public boolean pesquisarT3(int elemento) {
      boolean resp;
      int i = hashT3(elemento);

      if (t3[i] == NULO) {
         resp = false;
      } else if (t3[i] == elemento) {
         resp = true;
      } else {
         i = rehashT3(elemento);

         if (t3[i] == NULO) {
            resp = false;
         } else if (t3[i] == elemento) {
            resp = true;
         } else {
            resp = pesquisarArvore(raizArvoreT3, elemento);
         }
      }

      return resp;
   }

   public boolean pesquisarArvore(No no, int elemento) {
      boolean resp;

      if (no == null) {
         resp = false;
      } else if (no.elemento > elemento) {
         resp = pesquisarArvore(no.esq, elemento);
      } else if (no.elemento < elemento) {
         resp = pesquisarArvore(no.dir, elemento);
      } else {
         resp = true;
      }
      return resp;
   }

   public boolean pesquisarLista(int elemento) {
      boolean resp = false;
      for (Celula i = primeiroListaT2.prox; i != null; i = i.prox) {
         if (i.elemento == elemento) {
            resp = true;
            i = ultimoListaT2;
         }
      }
      return resp;
   }

   // remocao de elementos

   public boolean remover(int elemento) {
      boolean resp = false;
      int i = hashT1(elemento);

      if (t1[i] == NULO) {
         resp = false;
      } else if (t1[i] == elemento) {
         // Remove da T1
         t1[i] = NULO;
         resp = true;
      } else if (hashT2(elemento) == 0) {
         resp = removerT3(elemento);
      } else if (hashT2(elemento) == 1) {
         resp = removerLista(elemento);
      } else if (hashT2(elemento) == 2) {
         raizArvoreT2 = removerArvore(raizArvoreT2, elemento);
         resp = true;
      } else {
         System.out.println("ERRO!!!!");
      }

      return resp;
   }

   public boolean removerT3(int elemento) {
      boolean resp = false;
      int i = hashT3(elemento);

      if (t3[i] == elemento) {
         t3[i] = NULO;
         resp = true;
      } else {
         i = rehashT3(elemento);

         if (t3[i] == elemento) {
            t3[i] = NULO;
            resp = true;
         } else {
            raizArvoreT3 = removerArvore(raizArvoreT3, elemento);
            resp = true;
         }
      }
      return resp;
   }

   public boolean removerLista(int elemento) {
      boolean resp = false;

      Celula ant = primeiroListaT2;
      Celula i = primeiroListaT2.prox;

      while (i != null) {
         if (i.elemento == elemento) {
            ant.prox = i.prox;

            // Se removeu o último, atualiza ultimoListaT2
            if (i == ultimoListaT2) {
               ultimoListaT2 = ant;
            }

            resp = true;
            i = null; // "break" lógico
         } else {
            ant = i;
            i = i.prox;
         }
      }

      return resp;
   }

   public No removerArvore(No no, int elemento) {
      if (no == null) {
         return null;
      } else if (elemento < no.elemento) {
         no.esq = removerArvore(no.esq, elemento);
      } else if (elemento > no.elemento) {
         no.dir = removerArvore(no.dir, elemento);
      } else {
         // achou o elemento
         if (no.esq == null) {
            return no.dir;
         } else if (no.dir == null) {
            return no.esq;
         } else {
            // dois filhos → pega o menor da direita
            No aux = no.dir;
            while (aux.esq != null) {
               aux = aux.esq;
            }
            no.elemento = aux.elemento;
            no.dir = removerArvore(no.dir, aux.elemento);
         }
      }
      return no;
   }

}