 /*
 * =============================================================================
 * CONCLUSION DEL EQUIPO
 * Integrantes: Kevin Veláquez Hernández, Eduardo González Olvera
 *
 * Esta práctica fue una excelente actividad integradora de la implementación
 * del tema de los estados de las tareas, así como el uso de estructuras para la 
 * creación de distintas de estas partiendo desde una misma base y agilizando
 * así su producción.
 * =============================================================================
 */
#include "system_state.h"
#include "app_task.h"

void app_main(void)
{
	
	/*TODO --> Inicializar el sistema y crear tareas*/
	system_state_init();
	app_tasks_create();

}