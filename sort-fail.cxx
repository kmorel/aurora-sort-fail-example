#include <Kokkos_Core.hpp>
#include <Kokkos_DualView.hpp>
#include <Kokkos_Sort.hpp>

#include <iostream>

int main(int argc, char** argv)
{
  std::cout << "Calling initialize" << std::endl;
  Kokkos::initialize(argc, argv);

  constexpr int ARRAY_SIZE = 100;
  constexpr int OFFSET = 10;

  std::cout << "Creating unsorted data" << std::endl;
  std::vector<int> test_vector(static_cast<size_t>(ARRAY_SIZE));
  for (size_t i = 0; i < test_vector.size(); ++i)
  {
    test_vector[i] = OFFSET + ((ARRAY_SIZE - i) % 50);
    std::cout << " " << test_vector[i];
  }
  std::cout << std::endl;

  using KokkosViewCont =
    Kokkos::View<int*, Kokkos::LayoutRight, Kokkos::HostSpace, Kokkos::MemoryTraits<Kokkos::Unmanaged>>;
  using KokkosViewExec =
    decltype(Kokkos::create_mirror(Kokkos::DefaultExecutionSpace{}, KokkosViewCont{}));

  std::cout << "Copy to device" << std::endl;
  int* device_array = reinterpret_cast<int*>(
    Kokkos::kokkos_malloc<Kokkos::DefaultExecutionSpace::memory_space>(ARRAY_SIZE * sizeof(int)));
  {
  KokkosViewCont::const_type src_view(test_vector.data(), ARRAY_SIZE);
  KokkosViewExec dest_view(device_array, ARRAY_SIZE);
  Kokkos::deep_copy(dest_view, src_view);
  Kokkos::fence();
  }

  std::cout << "Create view" << std::endl;
  {
  KokkosViewExec view(device_array, ARRAY_SIZE);

  std::cout << "Call sort" << std::endl;
  Kokkos::fence();
  Kokkos::sort(view);
  Kokkos::fence();
  }

  std::cout << "Get results" << std::endl;
  {
  KokkosViewExec::const_type src_view(device_array, ARRAY_SIZE);
  KokkosViewCont dest_view(test_vector.data(), ARRAY_SIZE);
  Kokkos::deep_copy(dest_view, src_view);
  Kokkos::fence();
  }
  bool success = true;
  for (size_t i = 0; i < test_vector.size(); ++i)
  {
    std::cout << " " << test_vector[i];
    if (i > 0)
    {
      success &= test_vector[i-1] <= test_vector[i];
    }
  }
  std::cout << std::endl;

  Kokkos::finalize();

  if (success)
  {
    std::cout << "Sort succeeded.\n";
    return 0;
  }
  else
  {
    std::cout << "!!!!! Sort Failed!!!!!\n";
    return 1;
  }
}
