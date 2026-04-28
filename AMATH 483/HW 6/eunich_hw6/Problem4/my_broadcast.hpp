#ifndef MY_BROADCAST_HPP
#define MY_BROADCAST_HPP

#include <mpi.h>
#include <type_traits>


template<typename T>
struct mpi_type_trait { static MPI_Datatype type() { return MPI_DATATYPE_NULL; } };

template<> struct mpi_type_trait<int>    { static MPI_Datatype type() { return MPI_INT;    } };
template<> struct mpi_type_trait<float>  { static MPI_Datatype type() { return MPI_FLOAT;  } };
template<> struct mpi_type_trait<double> { static MPI_Datatype type() { return MPI_DOUBLE; } };


template <typename T>
void my_broadcast(T* data, int count, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    MPI_Datatype mtype = mpi_type_trait<T>::type();
    const int tag = 0;

    if (rank == root) {
        for (int r = 0; r < size; ++r) {
            if (r == root) continue;
            MPI_Send(data, count, mtype, r, tag, comm);
        }
    } else {
        MPI_Recv(data, count, mtype, root, tag, comm, MPI_STATUS_IGNORE);
    }
}

#endif 
