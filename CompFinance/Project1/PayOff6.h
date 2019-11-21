// Unchanged

#ifndef Payoff6_h
#define Payoff6_h

class PayOff
{
public:
    virtual double operator()(double Spot) const=0;
    virtual ~PayOff(){}
    virtual PayOff* clone() const=0;
private:
};

class PayOffCall : public PayOff
{
public:
    PayOffCall(double Strike);
    virtual double operator()(double spot) const override; // override essentially just checks that you actually are overriding (i.e. if you misspelled it will make you know)
    virtual ~PayOffCall(){};
    virtual PayOff* clone() const override;
private:
    double Strike; 
};

class PayOffPut : public PayOff
{
public:
    PayOffPut(double Strike);
    virtual double operator()(double Spot) const;
   virtual ~PayOffPut(){};
    virtual PayOff* clone() const;
private:
    double Strike;
};

#endif 
