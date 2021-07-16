
#include "rodos.h"
#include "support-libs.h"

#include "stdlib.h"

void printlist(const char* name, SortedList* list) {
    PRINTF("\n list %s (%d) \n", name, (int)list->getNumberOfElements());
    ITERATE_SORTED_LIST(*list)  {
        PRINTF(" %lld ", static_cast<long long>(iter->getSortField()));
    }
    PRINTF("\n------------\n");
}


SortedChainable elements[1000];
SortedChainable* temp;
SortedList freelista; SortedList* freelist = &freelista;

SortedList list01a; SortedList* list01 = &list01a;
SortedList list02a; SortedList* list02 = &list02a;
SortedList list03a; SortedList* list03 = &list03a;

void checkIt(const char *wo) {
      int numOfElems = freelist->getNumberOfElements() + list01->getNumberOfElements()
	+ list02->getNumberOfElements() + list03->getNumberOfElements();
     if(numOfElems != 1000) {
	PRINTF(" in %s es missing elements here %d\n", wo, numOfElems);
        PRINTF("elemts in listen = %d %d %d %d\n",
	      (int)freelist->getNumberOfElements(), (int)list01->getNumberOfElements(),
	      (int)list02->getNumberOfElements(),  (int)list03->getNumberOfElements() );
        PRINTF("last value  = %lld %lld %lld %lld\n",
	      static_cast<long long>(freelist->getLastValue()),
          static_cast<long long>(list01->getLastValue()),
	      static_cast<long long>(list02->getLastValue()),
          static_cast<long long>(list03->getLastValue()));
     }
}

void MAIN(){

   for(int i = 0; i < 1000; i++){
     elements[i].init();
     freelist->insert(&elements[i],(long long)rand()%100);
   }

   int64_t cnt = 0;

   while(true){

      bool ok;
      cnt++;
      if((cnt % 10000000) == 0)  {
         PRINTF(" cnt = %lld, elems = %d \n", static_cast<long long>(cnt), (int)(freelist->getNumberOfElements() + list01->getNumberOfElements()
		   + list02->getNumberOfElements() + list03->getNumberOfElements())  );
      }

      int wuerfel = rand() % 12;


      if (wuerfel == 0) list01->insert(&elements[rand()%1000],rand()%1000);
      if (wuerfel == 1) list02->insert(&elements[rand()%1000],rand()%1000);
      if (wuerfel == 2) list03->insert(&elements[rand()%1000],rand()%1000);

      
      if (wuerfel == 3){
          temp = freelist->getAndRemoveTheFirst();
          ok = list01->append(temp,list01->getLastValue());
          if(temp && !ok) PRINTF("append von %lx errro\n", (long)temp);
          checkIt("lsite01");
      }

      if (wuerfel == 4){
          temp = freelist->getAndRemoveTheFirst();
          ok = list02->append(temp,list02->getLastValue());
          if(temp && !ok) PRINTF("append von %lx error\n", (long)temp);
          checkIt("lsite02");
      }

      if (wuerfel == 5){
          temp = freelist->getAndRemoveTheFirst();
          ok = list03->append(temp,list03->getLastValue());
          if(temp && !ok) PRINTF("append von %lx error\n", (long)temp);
          checkIt("lsite03");
      }


      if (wuerfel == 6) list01->appendForce(&elements[rand()%1000],rand()%1000);
      if (wuerfel == 7) list02->appendForce(&elements[rand()%1000],rand()%1000);
      if (wuerfel == 8) list03->appendForce(&elements[rand()%1000],rand()%1000);

      if (wuerfel == 9)  freelist->appendForce(list01->getAndRemoveTheFirst(),0);
      if (wuerfel == 10) freelist->appendForce(list02->getAndRemoveTheFirst(),0);
      if (wuerfel == 11) freelist->appendForce(list03->getAndRemoveTheFirst(),0);


   }



}
