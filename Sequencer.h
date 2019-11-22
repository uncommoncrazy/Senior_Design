/*
 * Sequencer.h
 *
 *  Created on: Nov 18, 2019
 *      Author: Marco
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_
#define numberOfControls 5
extern void * sequencer[numberOfControls];
extern Uint16 controlIDs[numberOfControls];
extern Uint16 BPM;
extern Uint16 BeatSlices;
extern Uint16 Measures;
extern Uint16 BeatCount;
extern Uint16 CountPeriod;
void calulatePeriod();

#endif /* SEQUENCER_H_ */
