#include <stdint.h>
#include <stdlib.h>
#include "os.h"
#include <assert.h>

#define BITS_PER_LEVEL 9
#define OFFSET_MSB_BIT 11
#define STARTING_BIT_PTE 12
#define BITS_PER_PTE_FRAME 52
#define LAST_ENTRY 0

//Functions declarations
uint64_t * get_last_level(uint64_t pt, uint64_t vpn, int start, int update_flag);

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    uint64_t *pte;

    pte = get_last_level(pt, vpn, 36, 1);

    //updating the pte according to ppn
    if(ppn == NO_MAPPING) {
        *pte = 0;
    }
    else {
        *pte = ppn << 12; //Setting the pte to point to the given ppn
        *pte |= 1; //Changing the LSB to valid
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    uint64_t *pte;

    pte = get_last_level(pt, vpn, 36, 0);

    //if there is no mapping, then get_last_level return NULL pointer
    if(pte == NULL) {
        return NO_MAPPING;
    }

    //During update there was alloc for the page but not for this specific address
    //Or we set it to 0 in the update when ppn was NO_MAPPING
    if(*pte == 0) { 
        return NO_MAPPING;
    }

    return *pte >> 12;
}

//Function to get to the last level in the page table (where the ppn is located)
//If there is no mapping, then it returns NO_MAPPING
uint64_t * get_last_level(uint64_t pt, uint64_t vpn, int start, int update_flag){
    uint64_t *pte = NULL, *pt_pointer;
    uint64_t entry;


    while(start > LAST_ENTRY) {
        entry = (vpn >> start) & 0x1FF;
        pt_pointer = (uint64_t *)phys_to_virt(pt << 12); //Converting the physical page number (pt) to pointer to the memory
        pte = &pt_pointer[entry];


        if(pte == NULL) { //phys_to_virt failed
            exit(1);
        }
        
        if(!(*pte & 1)) { //Checking the LSB for validation - valid bit = 0
            if(update_flag) { //The function was called from update function
                assert(*pte = alloc_page_frame());
                *pte <<= 12;
                *pte |= 1; //Changing the lsb to valid
            }
            else { //The function was called from query function
                return NULL; //There is no mapping of the vpn
            }
        }

        pt = *pte >> 12;
        start -= BITS_PER_LEVEL;
    }

    entry = (vpn >> start) & 0x1FF;
    pt_pointer = (uint64_t *)phys_to_virt(pt << 12); //Converting the physical page number (pt) to pointer to the memory
    pte = &pt_pointer[entry];


    return pte;
}