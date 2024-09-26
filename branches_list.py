import ROOT

# Open the ROOT file
file = ROOT.TFile.Open("output.root")

# Access the tree inside the file (replace 'your_tree' with the actual tree name)
tree = file.Get("nominal_Loose")

# Open the output text file to save branches
with open("nominal_loose_branches.txt", "w") as outfile:
    # Loop over the branches in the tree
    for branch in tree.GetListOfBranches():
        # Get the branch name
        branch_name = branch.GetName()
        # Write the branch name to the text file
        outfile.write(f"{branch_name}\n")

# Close the ROOT file
file.Close()

# print("nominal_loose_branches.txt")
