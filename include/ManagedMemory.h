#ifndef MANAGEDMEMORY_H
#define MANAGEDMEMORY_H

class ManagedMemory {
    public:
        virtual void IncRefs() = 0;
        virtual void DecRefs() = 0;
        virtual void SelfDestruct() = 0;
        virtual ~ManagedMemory() {}
};

#endif // MANAGEDMEMORY_H
