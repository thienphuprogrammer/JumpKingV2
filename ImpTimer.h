#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_
#include "CommonFunction.h"

typedef struct sImpTimer ImpTimer;
struct sImpTimer 
{
	int start_tick_;
	int paused_tick_;
	bool is_paused_;
	bool is_started_;

	void (*start)(ImpTimer*);
	void (*stop)(ImpTimer*);
	void (*paused)(ImpTimer*);
	void (*unpaused)(ImpTimer*);

	int (*get_ticks)(ImpTimer*);

	bool (*is_started)(ImpTimer*);
	bool (*is_paused)(ImpTimer*);

};

ImpTimer ImpTimer_Create();
void ImpTimer_Destroy(ImpTimer* timer);

void ImpTimer_start(ImpTimer* timer);
void ImpTimer_stop(ImpTimer* timer);
void ImpTimer_paused(ImpTimer* timer);
void ImpTimer_unpaused(ImpTimer* timer);
int ImpTimer_get_ticks(ImpTimer* timer);

bool ImpTimer_is_started(ImpTimer* timer);
bool ImpTimer_is_paused(ImpTimer* timer);

#endif