#include <iostream>
#include <random>
#include <utility>
#include <vector>
#include <chrono>

int main(){

  // dimensione del vettore
  const unsigned int numElements= 100000000;   

  // creo il vettore con dimensione 0 e riservo lo spazio per dimensione numElements
  std::vector<int> input;
  input.reserve(numElements);

  // creo i generatori di numeri casuali
  std::mt19937 engine;
  std::uniform_int_distribution<> uniformDist(-5,5);

  // riempio il vettore partendo dalla sua fine aumentando la sua dimensione ad ogni elemento aggiunto
  for ( unsigned int i=0 ; i< numElements ; ++i) input.emplace_back(uniformDist(engine));

  // definisco la funzione di somma degli elementi di un vettore usando una lambda
  long long int sum= 0;
  auto f= [&](unsigned long long firstIndex, unsigned long long lastIndex){
    for (auto it= firstIndex; it < lastIndex; ++it){
        sum+= input[it];
    }
  };

  // eseguo la sommatoria degi elementi senza parallelizzazione e misuro il tempo di esecuzione
  auto start = std::chrono::system_clock::now();
  f(0,numElements);
  std::chrono::duration<double> dur= std::chrono::system_clock::now() - start;

  // stampo il risultato
  std::cout << "Time spent in reduction: " << dur.count() << " seconds" << std::endl;
  std::cout << "Sum result: " << sum << std::endl;
  
  return 0;
}
