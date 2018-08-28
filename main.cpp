#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

struct Processo {
    int magistrado;
    int horas;
    int empresa;
    int pos;
    Processo(int magistrado, int horas, int empresa, int pos) : magistrado(magistrado), horas(horas), empresa(empresa), pos(pos) {}
};

struct Node {
    Processo *P;
    Node *next;
    Node () : P(nullptr), next(nullptr) {}
};

struct List {
    Node *head;
    int size;
    List () : head(new Node()), size(0) {}
    void inserir (Node *head, Processo *P) {
        Node *cur = head;
        while (cur->next != nullptr && cur->next->P->horas <= P->horas) {
            cur = cur->next;
        }
        Node *N = new Node();
        N->P = P;
        N->next = cur->next;
        cur->next = N;
        ++this->size;
    }
    Processo *Pop(Node *head) {
        Processo *P = head->next->P;
        Node *n = head->next;
        head->next = n->next;
        delete n;
        this->size--;
        return P;
    }
};

struct Stack {
    Node *top;
    int size;
    Stack () : top(new Node()), size(0){}
    void Push(Node *top, Processo *P) {
        Node *N = new Node();
        N->P = P;
        N->next = top->next;
        top->next = N;
        ++this->size;
    };
    Processo *Pop(Node *top) {
        Processo *P = top->next->P;
        Node *n = top->next;
        top->next = n->next;
        delete n;
        this->size--;
        return P;
    }
    void Merge (Stack *a) {
        Node *cur = a->top;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        if(this->top->next != nullptr) {
            cur->next = this->top->next;
        }
        this->top = a->top;
        this->size =  this->size + a->size;
    }
};

void rec(Stack *lul, List *b, int omega, int HR) {
    if (lul->top->next->P->horas > omega) {
        lul->top->next->P->horas -= omega;
    } else {
        int r = omega - lul->top->next->P->horas;
        int aux = lul->top->next->P->horas;
        lul->top->next->P->horas += HR;
        HR += aux;
        delete &aux;
        b->inserir(b->head, lul->Pop(lul->top));
        if (lul->size != 0) {
            return rec (lul, b, r, HR);
        }
    }
}

struct Queue {
    Node *front;
    Node *rear;
    int size;
    Queue () : front(new Node()), rear(front), size(0) {}
    void Enqueue (Node *rear, Processo *P){
        Node *N = new Node();
        N->P = P;
        N->next = rear->next;
        rear->next = N;
        this->rear = N;
        ++this->size;
    };
    Processo *Dequeue (Node *front) {
        Processo *P = front->next->P;
        Node *n = front->next;
        front->next = n->next;
        delete n;
        this->size--;
        if (front->next != nullptr) {

        } else this->rear = front;
        return P;
    }
};


int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int E, N;
    int SN = 0;
    int ST = 0;
    cin >> E;
    Queue *F[E];
    for (int a = 0; a < E; a++) {
        cin >> N;
        ST += N;
        if (N > SN) {
            SN = N;
        }
        F[a] = new Queue();
        for (int b = 0; b < N; b++) {
            int J, T;
            cin >> J >> T;
            F[a]->Enqueue(F[a]->rear, new Processo(J,T,a,b));
        }
    }
    int M;
    cin >> M;
    Stack *H[M];
    int T[M];
    for (int d = 0; d < M ; d++) {
        H[d] = new Stack();
        cin >> T[d];
    }
    int auxm = 0;
    for (int c = 0; c < SN; c++) {
        for (int k = 0; k < E; k ++) {
            if (F[k]->size != 0) {
                H[auxm % M]->Push(H[auxm % M]->top, F[k]->Dequeue(F[k]->front));
            } else auxm--;
            auxm++;
        }
    }
    for (int e = 0; e < M; e++) {
        for (int f = 0; f < M; f++) {
            if (e != f) {
                Stack *SY = new Stack();
                Stack *SX = new Stack();
                while (H[e]->size != 0) {
                    Processo *K = H[e]->Pop(H[e]->top);
                    if (K->magistrado == f) {
                        SY->Push(SY->top, K);
                    } else {
                        if (K->magistrado == -1) {
                            K->magistrado = e;
                        }
                        SX->Push(SX->top, K);
                    }

                }
                H[e] = SX;
                if (SY->size != 0) {
                    H[f]->Merge(SY);
                }
            }
        }
    }
    List *OR = new List();
    bool stop = true;
    while (stop) {
        int aux = 0;
        for (int y = 0; y < M; y++) {
            if (H[y]->size != 0) {
                rec(H[y], OR, T[y], 0);
            } else aux++;
        }
        while (OR->size != 0) {
            Processo *end = OR->Pop(OR->head);
            cout << end->magistrado << " " << end->empresa << " " << end->pos << endl;
        }
        if (aux == M) {
            stop = false;
        }
    }
    return 0;
}
