#include"InstructionModifier.h"
#include"windowsUtillities.h"
#include"Utillities.h"

InstructionModifier::InstructionModifierDataEntry::InstructionModifierDataEntry(const std::array<unsigned char, 15>& savedinstruction, size_t sizeOfSavedInstruction) {
	std::memcpy(this->savedInstruction.data(), savedinstruction.data(), sizeOfSavedInstruction);
	this->sizeOfSavedInstruction = sizeOfSavedInstruction;
}

InstructionModifier::InstructionModifier(HANDLE processHandle) noexcept {
	this->processHandle = processHandle;
}

void InstructionModifier::changeInstruction(InstructionAddress_t instructionAddr, const std::array<char, 15>& newInstruction, size_t sizeOfChangedInstruction) {
	//this function doesn't saves history of changes.
	//An instruction that has been changed already cant be changed again until it will be restored to its previous state
	if (this->modifiedInstructions.find(instructionAddr) != this->modifiedInstructions.cend())
		return;

	std::array<unsigned char, 15> savedInstruction;
	bool err = ReadProcessMemory(this->processHandle, instructionAddr, savedInstruction.data(), sizeOfChangedInstruction, NULL);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());

	this->modifiedInstructions[instructionAddr] = InstructionModifier::InstructionModifierDataEntry(savedInstruction, sizeOfChangedInstruction);
	err = WriteProcessMemory(this->processHandle, instructionAddr, newInstruction.data(), sizeOfChangedInstruction, NULL);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());
	err = FlushInstructionCache(this->processHandle, instructionAddr, sizeOfChangedInstruction);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());
}

void InstructionModifier::restoreInstruction(InstructionAddress_t instructionAddr) {
	if (this->modifiedInstructions.find(instructionAddr) == this->modifiedInstructions.cend())
		CreateLogicError(L"this instruction address has not been modified by this controller");

	InstructionModifier::InstructionModifierDataEntry instructionModifiedEntry = this->modifiedInstructions[instructionAddr];
	bool err = WriteProcessMemory(this->processHandle, instructionAddr, instructionModifiedEntry.savedInstruction.data(), instructionModifiedEntry.sizeOfSavedInstruction, NULL);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());
	err = FlushInstructionCache(this->processHandle, instructionAddr, instructionModifiedEntry.sizeOfSavedInstruction);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());
	this->modifiedInstructions.erase(instructionAddr);
}