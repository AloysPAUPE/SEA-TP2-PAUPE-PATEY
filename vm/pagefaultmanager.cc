/*! \file pagefaultmanager.cc
Routines for the page fault managerPage Fault Manager
 * -----------------------------------------------------
 * This file is part of the Nachos-RiscV distribution
 * Copyright (c) 2022 University of Rennes 1.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details
 * (see see <http://www.gnu.org/licenses/>).
 * -----------------------------------------------------
*/

#include "vm/pagefaultmanager.h"
#include "kernel/msgerror.h"
#include "kernel/thread.h"
#include "vm/physMem.h"
#include "vm/swapManager.h"

PageFaultManager::PageFaultManager() {}

// PageFaultManager::~PageFaultManager()
/*! Nothing for now
 */
PageFaultManager::~PageFaultManager() {}

// ExceptionType PageFault(uint64_t virtualPage)
/*!
//	This method is called by the Memory Management Unit when there is a
//      page fault. This method loads the page from :
//      - read-only sections (text,rodata) $\Rightarrow$ executive
//        file
//      - read/write sections (data,...) $\Rightarrow$ executive
//        file (1st time only), or swap file
//      - anonymous mappings (stack/bss) $\Rightarrow$ new
//        page from the MemoryManager (1st time only), or swap file
//
//	\param virtualPage the virtual page subject to the page fault
//	  (supposed to be between 0 and the
//        size of the address space, and supposed to correspond to a
//        page mapped to something [code/data/bss/...])
//	\return the exception (generally the NO_EXCEPTION constant)
*/
ExceptionType
PageFaultManager::PageFault(uint64_t virtualPage) {
  #ifndef ETUDIANTS_TP
  printf("**** Warning: page fault manager is not implemented yet\n");
  exit(ERROR);
  #endif
  #ifdef ETUDIANTS_TP
    uint64_t free_page=g_physical_mem_manager->FindFreePage();
    TranslationTable* vp_translationTable = g_current_thread->GetProcessOwner()->addrspace->translationTable; 
    bool vp_bit_swap = vp_translationTable->getBitSwap(virtualPage);
    uint32_t vp_addr_disk = vp_translationTable->getAddrDisk(virtualPage);
    if (vp_bit_swap == true) {
        g_swap_manager->GetPageSwap(vp_addr_disk, free_page);
      } else if (vp_addr_disk == INVALID_SECTOR){
        // The section does not have an image in the executable
        // Fill it with zeroes
        memset(&(g_machine
                     ->mainMemory[vp_translationTable->getPhysicalPage(virtualPage) *
                                  g_cfg->PageSize]),
               0, g_cfg->PageSize);
      } else{
        // The section has an image in the executable file
        // Read it from the disk
        g_current_thread->GetProcessOwner()->exec_file->ReadAt(
            (char *) &(g_machine->mainMemory[vp_translationTable->getPhysicalPage(
                                                 virtualPage) *
                                             g_cfg->PageSize]),
            g_cfg->PageSize, vp_addr_disk);

      }
      vp_translationTable->setBitValid(virtualPage);
  #endif
  return ((ExceptionType) 0);
}
