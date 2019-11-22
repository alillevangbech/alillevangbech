#include "armadillo.hpp"


#include <cassert>
#include <fstream>
#include <iostream>

void LogReg(double alpha, double eps);

int main(int argc, const char *argv[]){
    
    LogReg(0.5 , 10.e-7);
    
    return 0;
}


void LogReg(double alpha, double eps)
{
    int nr_iterations = 0;
    //data
    arma::mat dataX, dataY , dataXtest;
    dataX.load("dataX.dat");
    dataY.load("dataY.dat");
    dataXtest.load("dataXtest.dat");
    
    arma::rowvec dataYtest(dataXtest.n_rows); //dataYtest for output
    arma::rowvec w = arma::zeros<arma::rowvec>(dataX.n_cols);
    
    double n = 1; // norm
    while (n > eps && nr_iterations < pow(10,5) ) {
        
        
        arma::rowvec LogReg_tmp_sum = arma::zeros<arma::rowvec>(dataX.n_cols);
        
        //calc gradient
        for (int i = 0; i < dataX.n_rows; i++)
        {
            
            LogReg_tmp_sum += dataY(i) * (1 / (1 + exp(dataY(i) * arma::dot(w, dataX.row(i))) )  ) * dataX.row(i);
            
        }
        LogReg_tmp_sum *= (-1.0) / (dataX.n_rows); //finished gradient
        
        //new guess
        n = arma::norm(LogReg_tmp_sum); //calc norm of gradient with current w
        w -= alpha*LogReg_tmp_sum; //update w
        
        
        if (nr_iterations % 1000  == 0) { //Show how we are doing after every 500 itr
            std::cout << "Norm:" << n << "iteration number:" << nr_iterations << "\n";
        }
        
        nr_iterations += 1;
        
    }
    arma::rowvec w_opt = w;
    std::cout <<  "number of itr" << nr_iterations << "res w:" << w_opt   << "\n" ;
    
    // Calc dataX . w_opt = tmpY
    
    arma::colvec tmpY;
    
    tmpY = dataXtest*w_opt.t();
    
    // assign according to sign
    for (int i = 0 ; i < tmpY.n_elem; i++) {
        if (tmpY(i) > 0 )
        {
            dataYtest(i) = 1;
        } else
            if(tmpY(i) < 0){
                dataYtest(i) = -1;
            }
            else {dataYtest(i) = 0;}
    }
    
    //write the output file
    std::ofstream write_output("LogReg.dat");
    assert(write_output.is_open());
    for (int i = 0 ; i < dataXtest.n_rows; i++) {
        write_output << dataYtest(i) << "\n";
    }
    write_output.close();
}


