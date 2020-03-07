#pragma once
#include<unordered_map>
#include<array>
#include<Windows.h>

class InstructionModifier {
public:
	using InstructionAddress_t = void*;
private:
	struct InstructionModifierDataEntry {
		std::array<char, 15> savedInstruction;
		size_t sizeOfSavedInstruction;

		InstructionModifierDataEntry(const std::array<char, 15>& savedinstruction, size_t sizeOfSavedInstruction);
		InstructionModifierDataEntry() = default;// created only to deal with unordered_map complain that this class doesn't have default ctor
	};

	std::unordered_map<InstructionAddress_t, InstructionModifierDataEntry> modifiedInstructions;
	HANDLE processHandle; //this handle will be used to change the instructions of the process the handle is belong to
public:

	InstructionModifier(HANDLE processHandle) noexcept;
	void changeInstruction(InstructionModifier::InstructionAddress_t instructionAddr, const std::array<char, 15>& newInstruction, size_t sizeOfChangedInstruction);
	void restoreInstruction(InstructionAddress_t instructionAddr);
};