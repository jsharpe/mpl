#include <cstdlib>
#include <iostream>
#include <mpl/mpl.hpp>

int main() {
  const mpl::communicator &comm_world=mpl::environment::comm_world();
  comm_world.barrier();
  std::cout << mpl::environment::processor_name() 
	    << " has passed barrier\n";
  comm_world.barrier();
  double x=0;
  if (comm_world.rank()==0) 
    x=10;
  comm_world.bcast(0, x);
  std::cout << "x = " << x << '\n';
  comm_world.barrier();
  std::vector<double> v(comm_world.size());
  x=comm_world.rank();
  comm_world.gather(0, x, v.data());
  if (comm_world.rank()==0) {
    for (int i=0; i<comm_world.size(); ++i) {
      std::cout << v[i] << '\t';
      v[i]=2*v[i]+1;
    }
    std::cout << '\n';
  }
  comm_world.scatter(0, v.data(), x);
  std::cout << "after scatter " << x << '\n';
  double y;
  comm_world.reduce(mpl::plus<double>(), 0, x, y);
  if (comm_world.rank()==0) 
    std::cout << "after reduce " << y << '\n';
  comm_world.allreduce(mpl::multiplies<double>(), x, y);
  std::cout << "after allreduce " << y << '\n';
  return EXIT_SUCCESS;
}
