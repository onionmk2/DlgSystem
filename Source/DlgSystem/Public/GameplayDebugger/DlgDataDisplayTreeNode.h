// Copyright 2017-2018 Csaba Molnar, Daniel Butum
#pragma once

#include "CoreMinimal.h"
#include "Reply.h"


class FDlgDataDisplayTreeNode;
typedef TSharedPtr<FDlgDataDisplayTreeNode> FDlgDataDisplayTreeNodePtr;

/* Base class node for all Nodes in the DlgDataDisplayWindow  */
class FDlgDataDisplayTreeNode : public TSharedFromThis<FDlgDataDisplayTreeNode>
{
	typedef FDlgDataDisplayTreeNode Self;

public:
	FDlgDataDisplayTreeNode(const FText& InDisplayText, TSharedPtr<Self> InParent);
	virtual ~FDlgDataDisplayTreeNode() {}

	/** Returns the display string for the row */
	FText GetDisplayText() const { return DisplayText; }

	// Parent:
	bool HasParent() const { return Parent.IsValid(); }
	TWeakPtr<Self> GetParent() const { return Parent; }
	Self* SetParent(TWeakPtr<Self> InParentNode)
	{
		Parent = InParentNode;
		return this;
	}

	// Children:
	const TArray<TSharedPtr<Self>>& GetChildren() const { return Children; }
	Self* AddChild(const TSharedPtr<Self>& ChildNode)
	{
		ensure(!ChildNode->IsRoot());
		ChildNode->SetParent(this->AsShared());
		Children.Add(ChildNode);
		return this;
	}
	void ClearChildren()
	{
		Children.Empty();
	}

	/** Is this the root node? Aka no parent. */
	bool IsRoot() const { return !Parent.IsValid(); }

	/** Is this the leaf node? Aka no children. */
	bool IsLeaf() const { return Children.Num() == 0; }

protected:
	/** Any children listed under this node. */
	TArray<TSharedPtr<Self>> Children;

	/** The display text for this item */
	FText DisplayText;

	/** The node that this is a direct child of (empty if this is a root node)  */
	TWeakPtr<Self> Parent;
};


/** Root node of the Dialogue Data Display */
class FDlgDataDisplayTreeRootNode : public FDlgDataDisplayTreeNode
{
	typedef FDlgDataDisplayTreeNode Super;
public:
	FDlgDataDisplayTreeRootNode();
};
