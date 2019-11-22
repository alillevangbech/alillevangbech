#include "armadillo.hpp"
#include <cassert>
#include <fstream>
#include <iostream>

int main()
{


arma::mat dataX, dataXtest;
arma::vec dataY;

   	dataX.load("dataX.dat");
    	dataY.load("dataY.dat");
    	dataXtest.load("dataXtest.dat");

	arma::uword RealnCol = dataX.n_cols;
	arma::uword RealnRow = dataX.n_rows;
	arma::uword TestnCol = dataXtest.n_cols;
	arma::uword TestnRow = dataXtest.n_rows;

	arma::mat d_i(RealnRow,TestnRow);
	
	arma::vec y(RealnRow);


	for (int i = 0; i < TestnRow; i++)
{
		for (int j = 0; j < RealnRow; j++)
		{
			d_i(j,i) = arma::norm(dataX.row(j) - dataXtest.row(i),2);
		}

	arma::vec k = d_i.col(i);
	arma::uvec sorta = sort_index(k);
	int temp_sum = 0;
	
	
		for (int h = 0; h < 5; h++)
		{
			temp_sum += dataY(sorta(h));
		}

	if (temp_sum > 0)
	{
		y(i) = 1;
	}
	else
	{
		y(i) = -1;
	}
}

    std::ofstream write_output("NN.dat");
    assert(write_output.is_open());
    for (int i = 0 ; i < dataXtest.n_rows; i++) {
        write_output << y(i) << "\n";
    }
    write_output.close();


			

		

	




}


