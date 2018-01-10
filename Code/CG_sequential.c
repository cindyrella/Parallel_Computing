#include<iostream>
#include<vector>
#include<sys/time.h>
/*
 * Computes the dot product for an array of size n
 * @pre the size of x and y is at least n
 */
double dot(std::vector<double>& x, std::vector<double>& y, int n){
    double resul = 0;
    for( int i = 0; i <n ; ++i){
        resul = resul + x[i] * y[i];
    }
    return resul;
}
/*
 * zaxpy: computes the sum z = alpha*x + y
 * for A sparse
 * @param[in] alpha
 * @param[in] x vector
 * @param[in] y vector
 * @param[out] z vector
 * @param[in] n size of the arrays
 * @pre x, y, z have the same size n
 * @pre we can override in x and y
 */
void zaxpy(double alpha,std::vector<double>& x, std::vector<double>& y,
           std::vector<double>& z, int n){
    for(int i = 0; i<n; ++i){
        z[i] = alpha * x[i] + y[i];
    }
}

//Defines the sparse representation of a laplacian
//in a uniform grid
struct laplace{
  private:
    int n_x;
    int n_y;
    int n;
    double d_x;
    double d_y;
  public:
    /*
     * Constructor: only needs n_grid
     */
    laplace(int n_x_,int n_y_):
            n_x(n_x_), n_y(n_y_), d_x(double((n_x_-1)* (n_x_-1))),
            d_y(double((n_y_-1)* (n_y_-1))), n(n_x_*n_y_) {}

    /*
     * zaAxpy: computes the product z = A*x
     * for A sparse
     * @param[in] alpha
     * @param[in] x vector
     * @param[in] y vector
     * @param[out] z vector
     * @pre we assume the ordering of the vector is by rows
     *      x(0,0) x(0,1) ... x(0,n-1), x(1,0), ...
     * @pre we assume x contains also the boundary points
     * @pre the mesh including the boundary points is (n)x(n)
     * @pre we can not override in x
     */
    void Ax(std::vector<double>& x,std::vector<double>& z){
        int n_row = 0;
        int id = 0;
        for( int id_x = 1; id_x < (n_x - 1); ++id_x){
            n_row = id_x * n_y ;
            for( int id_y = 1; id_y < (n_y - 1); ++id_y){
               id = n_row + id_y;
                 z[id] = (-2.) * (d_x + d_y) * x[id] +
                         d_x * (x[id - n_y] + x[id + n_y]) +
                         d_y * (x[id + 1] + x[id - 1]);
            }
        }
    }
    /*
     * CG_Solver: computes the CG solver for the laplace matrix
     * @param[in] b LHS
     * @param[in] u_0 initial guess, it will also contain the solution
     * @param[in] tol error to stop
     */

    int CG_solver(std::vector<double>& b,std::vector<double>& u, double tol){
        std::vector<double> r (n,0);
        std::vector<double> Ap (n,0);
        Ax(u,Ap);
        zaxpy(-1, Ap, b, r,n);
        double res = dot(r,r,n);
        double res_0 = res;
        double new_res, alpha, beta;
        //std::vector<double> p (r.begin(), r.end());
        std::vector<double> p (n,0);
        p = r;
        for(int iter = 0; iter < n; ++iter){
            Ax(p,Ap);
            alpha = res / dot(p,Ap,n);
            zaxpy(alpha,p,u,u,n);
            zaxpy(-1. * alpha, Ap, r, r,n);
            new_res = dot(r,r,n);
            if (new_res/res_0 < tol){
              return iter + 1;
            }
            beta = new_res / res;
            res = new_res;
            zaxpy(beta, p, r, p, n);
        }
        return n;

    }

};
int main(int argc, char* argv[])
{
    struct timeval time;
    gettimeofday(&time,NULL);
    double t1 = (double) time.tv_sec + 0.000001*((double) time.tv_usec);

    double k = 1.0;
    int n_x = 129;
    int n_y = 129;
    double eps = 1e-24;
    laplace A(n_x,n_y);

    std::vector<double> b(n_x*n_y,k);
    std::vector<double> u_0(n_x*n_y,0);
    //Impose initial conditions
    for(int i=0;i<(n_x*n_y);i=i+(n_x-1)*n_y){
        for(int j = 0; j<n_y; ++j){
            b[i+j]=0;
        }
    }
    for(int i=1;i<(n_x-1);++i){
        for(int j = 0; j<n_y; j=j+n_y-1){
            b[i*n_y+j]=0;
        }
    }

    int iter = A.CG_solver( b,u_0, eps);
    gettimeofday(&time,NULL);
    double t2 = (double) time.tv_sec + 0.000001*((double) time.tv_usec);

    std::cout << "Iter is " << iter << std::endl;
    std::cout << "f(n/2,n/2)=" << u_0[(n_x-1)/2 * n_y + (n_y-1)/2] << std::endl;
    std::cout << "Time "<< (t2 - t1)*1000<< "ms"<<std::endl;
    return 0;
}
