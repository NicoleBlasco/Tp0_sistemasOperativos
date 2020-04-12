/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"

	log_info(logger, "soy un log");

	config = leer_config();

	ip =  config_get_string_value(config, "IP");
	puerto =  config_get_string_value(config, "PUERTO");


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	conexion = crear_conexion(ip, puerto);

	enviar_mensaje("Que onda viejo?", conexion);

	log_info(logger, recibir_mensaje(conexion));

	terminar_programa(conexion, logger, config);


}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("tp0.log", "tp0 GAme and Watch", true, LOG_LEVEL_INFO);

}


//TODO
t_config* leer_config(void)
{
	return config_create("tp0.config");
}

//TODO

// Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config)
// con las funciones de las commons y del TP mencionadas en el enunciado

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	return;

}
