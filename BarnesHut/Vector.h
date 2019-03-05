#pragma once
template<typename T, size_t DIM> struct Vector {
	T v[DIM];
	Vector() {
		for (size_t i = 0; i < DIM; i++)
			v[i] = 0;
	}
	Vector operator+(const Vector& vector) {
		Vector ret;
		for (size_t i = 0; i < DIM; i++)
		{
			*(ret.v + i) = *(this->v + i) + *(vector.v + i);
		}
		return ret;
	}
	Vector operator* (float f) {
		Vector ret;
		for (size_t i = 0; i < DIM; i++)
		{
			*(ret.v + i) = *(this->v + i) * f;
		}
		return ret;
	}
};