#pragma once

#include <cmath>
#include <string>
#include <map>
#include <functional>
#include <fstream>
#include <iostream>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "structs.hpp"

std::vector<unsigned char> calculate_md5_hash(const std::vector<char>& msg);
long int greatestCommonDivisor(long int e, long int t);
long int calculateE(long int t);
long long int fexp(long long int base, long long int exp, long long int mod);
upint fexp_rsa(upint base, upint exp, upint mod);
int isPrime(int n);
long long gcd(long long a, long long b, long long* x, long long* y);
int gcd(long a, long b, long* x, long* y);
std::vector<int> solve(int a, int y, int m);
int generate_numbers(long int& c, long int& d, long int p);
long long int generate_prime_numbers(long long int& c, long long int& d, long long int p);
long int mul(long int a, long int b, long int p);
int ShamirAlg(std::string filepath);
std::string ElgamalEncrypt(std::string filepath, ElgamalKeys& keys);
void ElgamalDecrypt(std::string filepath, ElgamalKeys& keys);
void ElgamalInit(ElgamalKeys& keys);
void VernamAlg(std::string filepath);
void RSAAlg(std::string filepath);

// long take_hash(std::vector<char>& msg);
void RSA_Get_keys(RSA_Open_key& open_key, RSA_Secret_key& secret_key);
RSA_Signed_msg RSA_Sign_msg(const std::vector<char>& msg, RSA_Open_key open_key, RSA_Secret_key secret_key);
bool RSA_Check_msg(RSA_Signed_msg& e, RSA_Open_key open_key);
long int get_inverse(long long int& c, long long int p);
void Elgamal_Get_keys(Elgamal_Open_key& open_key, Elgamal_Secret_key& secret_key);
Elgamal_Signed_msg Elgamal_Sign_msg(const std::vector<char>& msg, const Elgamal_Open_key& open_key, const Elgamal_Secret_key& secret_key);
bool Elgamal_Check_msg(const Elgamal_Signed_msg& e, const Elgamal_Open_key& open_key);
void Gost_Get_keys(Gost_Open_keys& keys, pint& x, std::vector<char>& msg);
Gost_Signed_msg Gost_Sign_msg(std::vector<char>& msg, Gost_Open_keys keys, pint x);
bool Gost_Check_msg(Gost_Signed_msg e, Gost_Open_keys keys);
pint gcd(pint a, pint b, pint* x, pint* y);
pint mod_pow(pint num, pint degree, pint p);
pint take_hash_pint(std::vector<char>& msg);