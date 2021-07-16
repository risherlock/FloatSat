// Lizenz: public domain

typedef void (*constructor)();

// Im Linkerskript definiert
extern constructor __init_array_start;
extern constructor __init_array_end;

int __cxa_atexit(void(*f)(void *), void *p, void *d);
void __cxa_pure_virtual(void);
int __cxa_guard_acquire(unsigned char *g);
void __cxa_guard_release(unsigned char *g);

// Ruft die Konstruktoren fuer globale/statische Objekte auf
void call_constructors();
void call_constructors() {
	constructor* i;
	unsigned int j;
	for (i = &__init_array_start; i != &__init_array_end; ++i) {
		(*i)();
		j++;
	}
	j++;
	j--;
}

void *__dso_handle = 0;

// wird am Ende des Konstruktors globaler Objekte aufgerufen und speichert Destruktoradresse fuer spaeteren Aufruf
// -> Standardimplementierung fuerht zu Absturz, deswegen hier neu implementiert
// -> keine Funtionalituet implementiert, weil Destruktoren globaler Objekte in
//    RODOS nie ausgefuehrt werden
// Anm: Diese Funktionalitaet kann beim Kompilieren des Compilers deaktiviert werden, indem
//      man --enable__cxa_atexit nicht angibt. Dann werden die Destruktoradressen beim Kompilieren
//      der embedded SW in eine eigene Section gepackt (.dtors) und koennen genauso wie die Konstruktoren
//      aufgerufen werden.
int __cxa_atexit(void(*f)(void *), void *p, void *d) {
	return 0;
}

// Errorhandler, der ausgefuehrt wird, wenn eine "pure virtual"
// Funktion aufgerufen wird
void __cxa_pure_virtual(void) {
	while(1){}
}

int __cxa_guard_acquire(unsigned char *g) {
	if( *g == 0 ) {
		*g = 1;
		return 1;	// success
	} else {
		return 0;
	}
}

void __cxa_guard_release(unsigned char *g) {
	*g = 2;
}
