#include <set>
#include <gmsh.h>
#include <iostream>
#include <vector>
#include <math.h>
#define PI 3.1415927
const int N = 3;

struct point{
    float x;
    float y;
    float z;
};

int main(int argc, char **argv){
  gmsh::initialize();

  gmsh::model::add("t3");

  float lc = 1e-1;
  float R = 2.0;
  float r = 1.0;
  
  std::vector <point> lp;

  float t;
  t = (float) (2*PI)/N;

  for (int i=0; i<N; i++){
    for (int j=0; j<N; j++){
      float phi = t*i;
      float psi = t*j;  

      point p;
      p.x = (R + r*cos(psi))*cos(phi);
      p.y = (R + r*cos(psi))*sin(phi);
      p.z = r*sin(psi);

      lp.push_back(p); 
    }
  }
  
  for (int i=0; i<N*N; i++){
    gmsh::model::geo::addPoint(lp[i].x, lp[i].y, lp[i].z, lc, i);
  }
  
  for (int i=0; i<N; i++){
    for (int j=0; j<N; j++){
      gmsh::model::geo::addLine(i*N+j, i*N+((j+1)%N), i*N+j);
    }
  }

  for (int i=0; i<N; i++){
    for (int j=0; j<N; j++){
      gmsh::model::geo::addLine(i*N+j, ((i+1)%N)*N+j, N*N+i*N+j);
    }
  }


  for (int i=0; i<N; i++){
    for (int j=0; j<N; j++){
      //std::cout << (i*N+j) << i*N+j+N*N << ((i+1)%N)*N+j << (i*N+(j+1)%N+N*N) << std::endl;
      if ((i== 0)&&(j==0)){
        continue;
      }
      gmsh::model::geo::addCurveLoop({-(i*N+j), (i*N+j+N*N), ((i+1)%N)*N+j, -(i*N+(j+1)%N+N*N)}, i*N+j);
    }
  }
  gmsh::model::geo::addCurveLoop({0, N*N+1, -N, -N*N}, N*N);

  for (int i=0; i<N; i++){
    for (int j=0; j<N; j++){
      if ((i== 0)&&(j==0)){
        continue;
      }
      gmsh::model::geo::addPlaneSurface({i*N+j}, i*N+j);
    }
  }
  gmsh::model::geo::addPlaneSurface({N*N}, N*N);


  gmsh::model::geo::synchronize();

  gmsh::model::mesh::generate(2);

  gmsh::write("t3.msh");

  std::set<std::string> args(argv, argv + argc);
  if(!args.count("-nopopup")) gmsh::fltk::run();

  gmsh::finalize();
  
  lp.clear();
  return 0;
}
