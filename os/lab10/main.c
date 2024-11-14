#include <stdio.h>
#include <windows.h>

const char* GetMemoryStateString(DWORD state);

int main() {
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);
  printf("Page size is %u bytes\n", sys_info.dwPageSize);

  SIZE_T mem_size = 1 << 20;
  void* memory = VirtualAlloc(NULL, mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  
  if (memory == NULL) {
    printf("Failed to allocate memory\n");
    return 1;
  }

  printf("Allocated 1MiB of memory\n");

  MEMORY_BASIC_INFORMATION mbi;
  if (VirtualQuery(memory, &mbi, sizeof(mbi)) == 0) {
    printf("Failed to get memory state!\n");
    return 1;
  }

  printf("State after allocation: %s\n", GetMemoryStateString(mbi.State));

  if (!VirtualFree(memory, 0, MEM_RELEASE)) {
    printf("Failed to free memory\n");
    return 1;
  }
  printf("Memory deallocated.\n");

  if (VirtualQuery(memory, &mbi, sizeof(mbi)) == 0) {
    printf("Failed to get memory state after freeing\n");
    return 1;
  }

  printf("State after deallocation: %s\n", GetMemoryStateString(mbi.State));

  return 0;
}

const char* GetMemoryStateString(DWORD state) {
  switch (state) {
    case MEM_COMMIT:
      return "Committed";
    case MEM_RESERVE:
      return "Reserved";
    case MEM_FREE:
      return "Free";
    default:
      return "None";
  }
}

