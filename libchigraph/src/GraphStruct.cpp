#include "chi/GraphStruct.hpp"
#include "chi/Context.hpp"
#include "chi/GraphModule.hpp"

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <llvm/IR/DerivedTypes.h>

namespace chi {

GraphStruct::GraphStruct(GraphModule& mod, std::string name)
    : mModule{&mod}, mContext{&mod.context()}, mName{std::move(name)} {}

void GraphStruct::addType(DataType ty, std::string name, size_t addBefore) {
	Expects(addBefore <= types().size() && ty.valid() && !name.empty());

	mTypes.emplace_back(name, ty);

	// invalidate the current DataType
	mDataType = {};
}

void GraphStruct::modifyType(size_t id, DataType newTy, std::string newName) {
	Expects(id < types().size() && newTy.valid() && !newName.empty());

	mTypes[id] = {std::move(newName), std::move(newTy)};

	// invalidate the current DataType
	mDataType = {};
}

void GraphStruct::removeType(size_t id) {
	Expects(id < types().size());

	mTypes.erase(mTypes.begin() + id);

	// invalidate the current DataType
	mDataType = {};
}

DataType GraphStruct::dataType() {
	// if we have already calculated this, use that
	if (mDataType.valid()) { return mDataType; }

	if (types().empty()) { return {}; }

	// create llvm::Type
	std::vector<llvm::Type*> llTypes;
	llTypes.reserve(types().size());

	std::vector<llvm::Metadata*> diTypes;
	diTypes.reserve(types().size());

	size_t currentOffset = 0;
	for (const auto& type : types()) {
		auto debugType = type.type.debugType();

		llTypes.push_back(type.type.llvmType());

		auto member =
		    llvm::DIDerivedType::get(context().llvmContext(), llvm::dwarf::DW_TAG_member,
#if LLVM_VERSION_MAJOR <= 3 && LLVM_VERSION_MINOR <= 8
		                             llvm::MDString::get(context().llvmContext(), type.name),
#else
		                             type.name,
#endif
		                             nullptr, 0, nullptr, debugType, debugType->getSizeInBits(), 8,
		                             currentOffset, llvm::DINode::DIFlags{}, nullptr);
		diTypes.push_back(member);

		currentOffset += debugType->getSizeInBits();
	}
	auto llType = llvm::StructType::create(llTypes, name());

	auto diStructType = llvm::DICompositeType::get(
	    context().llvmContext(), llvm::dwarf::DW_TAG_structure_type, name(), nullptr, 0, nullptr,
	    nullptr, currentOffset, 8, 0, llvm::DINode::DIFlags{},
	    llvm::MDTuple::get(context().llvmContext(), diTypes), 0, nullptr, {}, "");

	mDataType = DataType(&module(), name(), llType, diStructType);

	return mDataType;
}

}  // namespace chi
