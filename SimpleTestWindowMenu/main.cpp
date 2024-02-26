#include "allegro5/allegro.h"


int main()
{

	al_init();
	ALLEGRO_DISPLAY *main_display = nullptr;
	main_display = al_create_display(1000,1000);
	al_flip_display();
	al_rest(5.0);
	al_destroy_display(main_display);
}