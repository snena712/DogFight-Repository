#pragma once

class Config
{
private:	
	bool m_reverseX = false;
	bool m_vib = false;
public:
	static Config* Instance() {
		static Config singleton;
		return &singleton;
	}
	Config() {};
	~Config() {};

	void SetReverseX(bool _flg) { m_reverseX = _flg; }
	bool GetReverseX() {return m_reverseX;}

	void SetControllerVib(bool _flg) { m_vib = _flg; }				// バイブ
	bool GetControllerVib() { return m_vib; }				// バイブ
};

