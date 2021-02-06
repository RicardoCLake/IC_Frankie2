#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <cassert>
#include <condition_variable>


using namespace std;

class Semaphore {

    private:
        /* Número de threads que estão querendo acessar o recurso */
        unsigned int n_threads = 0;
        /* Mutex para sinalizar seção crítica */
        mutex mtx;
        /* Controle de thread pronta a executar */
        condition_variable ready;

    public:
        /*
         * Coloca a thread para dormir até que a variável ready envie um sinal
         */
        void wait () {
            unique_lock<mutex> lock(mtx);

            /* Esse loop não tem espera ocupada. A thread dorme (wait state) */
            while(!n_threads) {
                ready.wait(lock);
            }

            n_threads--;
        }

        /*
         * Incrementa o número de threads e notifica uma thread para acordar
         */
        void go () {
            unique_lock<mutex> lock(mtx);

            n_threads++;
            ready.notify_one();
        }
};

class SimpleThread {

    private:
        /* Semáforo de execução das threads */
        Semaphore semaphore;

        /*
         * Entra em espera por um período de tempo entre 0 e 5 segundos. Ao
         * final imprime uma mensagem com o número da thread atual
         */
        void HelloMessage (int thread_no) {

            this_thread::sleep_for(chrono::milliseconds(rand() % 5000));

            semaphore.wait();
            cout << "Hello from thread: " << thread_no << endl;
            semaphore.go();
        }

    public:

        /* Número máximo de threads */
        int N_THREADS = 20;

        /*
         * Cria uma thread através do módulo std::thread do C++
         */
        thread *spawn (int thread_no) {
            semaphore.go();
            return new thread(&SimpleThread::HelloMessage, this, thread_no);
        }
};

int
main (int argc, char* argv[])
{
    /* Cria-se um objeto simples da nossa classe de Threads */
    SimpleThread *simple_thread = new SimpleThread();
    /* Um array para controlar a lista de threads abertas */
    vector<thread *> threads(simple_thread->N_THREADS);

    /* Cria uma thread para cada elemento do array */
    for(int i = 0; i < simple_thread->N_THREADS; i++) {
        threads[i] = simple_thread->spawn(i);
    }
    /* Para cada thread aguarda o final de sua execução */
    for(int i = 0; i < simple_thread->N_THREADS; i++) {
        threads[i]->join();
    }

    return EXIT_SUCCESS;
}