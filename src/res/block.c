#include "block.h"

bool block_print(block_t* block)
{
	if(block == NULL)
		return false;

	printf("\nn: %d\ne: %d\ns: %d\nw: %d\nt: %d\nb: %d\nnmod: %d\nemod: %d\nsmod: %d\nwmod: %d\ntmod: %d\nbmod: %d\nunknown[0]: %d\nunknown[1]: %d\nunknown[2]: %d\nunknown[3]: %d\n", block->n, block->e, block->s, block->w, block->t, block->b, block->nmod, block->emod, block->smod, block->wmod, block->tmod, block->bmod, block->unknown[0], block->unknown[1], block->unknown[2], block->unknown[3]);
	return true;
}
