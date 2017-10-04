/*
 * DAC.h
 *
 *  Created on: 26/09/2017
 *      Author: eric_
 */
#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#ifndef DAC_H_
#define DAC_H_


/*!
 	 \brief	 Esta funcion inicializara el DAC, Pondra el voltaje de referencia en el DACREF_2,
 	 	 	se inicializara en modo de alto consumo  y activara su clock gating.
 	 \return void
 	 \todo Inicia lo necesario para que el DAC pueda recibir datos.
 */
void DACSetup();

/*!
 	 \brief	 Esta funcion recibira los valores digitales para pasar a analogos.
 	 \in[TableL] Arreglo de uint8 que pasa a la parte baja de los valores del DAC
 	 \in[TableH] Arreglo de uint8 que pasa a la parte alta de los valores del DAC
 	 \return void
 	 \todo Carga los datos a convertir en el DAC0
 */
void DACLoad(uint8 TableL, uint8 TableH);
/*!
 	 \brief	 Activara el clock gating del DAC0
 	 \return void
 	 \todo Activa el clock gating del DAC0
 */
void DAC_ClockGating();
#endif /* DAC_H_ */
