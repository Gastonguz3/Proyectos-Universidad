#include "task_lib.h"

#define WIDTH TASK_VIEWPORT_WIDTH
#define HEIGHT TASK_VIEWPORT_HEIGHT

#define SHARED_SCORE_BASE_VADDR (PAGE_ON_DEMAND_BASE_VADDR + 0xF00)
#define CANT_PONGS 3

#define TASK_ID_PONG_1 0
#define TASK_ID_PONG_2 1
#define TASK_ID_PONG_3 2

void task(void) {
	screen pantalla;
	// ¿Una tarea debe terminar en nuestro sistema?
	while (true)
	{
		
		uint32_t* task_record_0 = (uint32_t*) SHARED_SCORE_BASE_VADDR + ((uint32_t) TASK_ID_PONG_1 * sizeof(uint32_t)*2);
		uint32_t* task_record_1 = (uint32_t*) SHARED_SCORE_BASE_VADDR + ((uint32_t) TASK_ID_PONG_2 * sizeof(uint32_t)*2);
		uint32_t* task_record_2 = (uint32_t*) SHARED_SCORE_BASE_VADDR + ((uint32_t) TASK_ID_PONG_3 * sizeof(uint32_t)*2);

		uint32_t score_player_1_1 = task_record_0[0]; 
		uint32_t score_player_2_1 = task_record_0[1];

		uint32_t score_player_1_2 = task_record_1[0];
		uint32_t score_player_2_2 = task_record_1[1];
		
		uint32_t score_player_1_3 = task_record_2[0];
		uint32_t score_player_2_3 = task_record_2[1];
		
	// Completar:
	// - Pueden definir funciones auxiliares para imprimir en pantalla
	// - Pueden usar `task_print`, `task_print_dec`, etc. 
		
		task_print(pantalla, "jugador 1", WIDTH/2 - 10, 2*HEIGHT/6, C_FG_RED);
		task_print(pantalla, "jugador 2", WIDTH/2 + 1, 2*HEIGHT/6, C_FG_RED);

		task_print(pantalla, "G 1", 3, 3*HEIGHT/6, C_FG_LIGHT_RED);
		task_print(pantalla, "G 2", 3, 4*HEIGHT/6, C_FG_LIGHT_RED);
		task_print(pantalla, "G 3", 3, 5*HEIGHT/6, C_FG_LIGHT_RED);

		task_print_dec(pantalla, score_player_1_1, 2, WIDTH/2 - 7, 3*HEIGHT/6, C_FG_MAGENTA);
		task_print_dec(pantalla, score_player_2_1, 2, WIDTH/2 + 4, 3*HEIGHT/6, C_FG_MAGENTA);

		task_print_dec(pantalla, score_player_1_2, 2, WIDTH/2 - 7, 4*HEIGHT/6, C_FG_MAGENTA);
		task_print_dec(pantalla, score_player_2_2, 2, WIDTH/2 + 4, 4*HEIGHT/6, C_FG_MAGENTA);

		task_print_dec(pantalla, score_player_1_3, 2, WIDTH/2 - 7, 5*HEIGHT/6, C_FG_MAGENTA);
		task_print_dec(pantalla, score_player_2_3, 2, WIDTH/2 + 4, 5*HEIGHT/6, C_FG_MAGENTA);

		syscall_draw(pantalla);
	}
}
