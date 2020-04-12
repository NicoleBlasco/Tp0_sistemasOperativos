/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	*bytes = sizeof(int) *2 + paquete->buffer->size;

	void * magic = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 		// Verifica si usamos IPv4 o IPv6 (sistemas de direccionamiento, protocolos fundamentales de Internet)
	hints.ai_socktype = SOCK_STREAM; 	// Indica que usaremos el protocolo TCP que sirve para la conexion
	hints.ai_flags = AI_PASSIVE;

	//sockets de tipo stream --> garantiza que el msj llegará --> SOCK_STREAM
	//sockets no orientados a la conexion --> protocolo UDP que garantiza que los datos que llegan son correctos pero no que lleguen todos. --> SOCK_DGRAM




	getaddrinfo(ip, puerto, &hints, &server_info);

	//	&hints 		  -> auxDir: dirección de una estructura auxiliar estática en la que se debe determinar la familia de protocolos y el protocolo a utilizar.
	//	&server_info  -> dirStruct: dirección del puntero de la estructura dinámica final que utilizaremos para crear el socket.

	// Carga en socket_cliente los datos de la conexion

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	//Para llenar el buffer --> con mallok inicializamos el puntero al buffer -> nos garantiza una zona de memoria concedida (que no esté ocupada)
	t_buffer* buffer=malloc(sizeof(t_buffer));

//	typedef struct
//	{
//		int size;  		--> el tamaño del buffer: tamaño del mensaje + 1 (caracter centinela '\0')
//		void* stream;	--> siendo el stream el canal
//	} t_buffer;

	buffer->size =strlen(mensaje)+1;
	buffer->stream=malloc(buffer->size);

	memcpy(buffer->stream, mensaje, buffer->size);

	t_paquete* paquete = malloc(sizeof(paquete));

//	typedef struct
//	{
//		op_code codigo_operacion;
//		t_buffer* buffer;
//	} t_paquete;

	paquete->codigo_operacion= MENSAJE;
	paquete->buffer=malloc(buffer->size + sizeof(buffer->size));

	// Cargo el paquete -> buffer
	memcpy(paquete->buffer, buffer, buffer->size + sizeof(buffer->size));

	// Tengo listo el paquete -->lo serializo

	int cant_bytes;

	void* magic= serializar_paquete(paquete,&cant_bytes);

	int status = send(socket_cliente, magic,cant_bytes,0); //ponerlo en una variable el tamaño

	free(magic);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return;
}

//TODO
char* recibir_mensaje(int socket_cliente)
{
	//Siempre tengo que reservar memoria cuando tenga puntero

	t_paquete* paquete_recibido = malloc(sizeof(paquete_recibido));

	int status = recv(socket_cliente,&(paquete_recibido->codigo_operacion),sizeof(int),0);

	//recibimos el buffer

	paquete_recibido->buffer=malloc(sizeof(paquete_recibido->buffer));
	status = recv(socket_cliente,&(paquete_recibido->buffer->size),sizeof(int),0);
	paquete_recibido->buffer->stream = malloc(paquete_recibido->buffer->size);
	status = recv(socket_cliente,(paquete_recibido->buffer->stream),(paquete_recibido->buffer->size),0);

	switch(paquete_recibido->codigo_operacion){
		case MENSAJE:
	    return paquete_recibido->buffer->stream; //Retorna el mje recibido y sale de la funcion recibir_mensaje

	}
		return "Me mandaron cualquier cosa"; //en caso que me hayan enviado algo que no es un mensaje, la funcion retorna esto.
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
