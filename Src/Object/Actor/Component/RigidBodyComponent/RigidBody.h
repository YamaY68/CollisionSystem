#pragma once

#include<DxLib.h>

class RigidBody
{
public:
	enum class BodyType
	{
		Static,		//静的
		Dynamic,	//動的
		Kinematic	//キネマティック
	};

	void SetBodyType(BodyType type);		//ボディタイプ設定
	void SetMass(float mass);				//質量設定//逆質量も同時に設定
	void SetUseGravity(bool useGravity);		//重力使用設定
	void SetVelocity(const VECTOR& velocity);	//速度設定
	void AddForce(const VECTOR& addForce);		//力を加える
	void setvelocity(const VECTOR& velocity) { velocity_ = velocity; } //速度設定
	void Setgrounded(bool isGrounded) { isGrounded_ = isGrounded; } //接地設定

	BodyType GetBodyType() const;		//ボディタイプ取得
	float GetMass() const;				//質量取得
	float GetInvMass() const;		  //逆質量取得
	bool IsUseGravity() const;			//重力使用取得
	VECTOR GetVelocity() const;			//速度取得
	VECTOR GetForce() const { return force_; }	//力取得
	bool IsGrounded() const { return isGrounded_; } //接地取得

	void ClearForce();				//力をクリア

	
private:
	BodyType type_ = BodyType::Dynamic;
	
	float mass_ = 1.0f;			//質量
	float invMass_ = 1.0f;		//逆質量

	VECTOR velocity_ = VGet(0.0f, 0.0f, 0.0f);		//速度
	VECTOR force_ = VGet(0.0f, 0.0f, 0.0f);		//力
	bool useGravity_ = true;		//重力を使うかどうか
	bool isGrounded_ = false;		//接地しているかどうか
};

