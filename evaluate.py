#!/usr/bin/env python3
"""
Hash Code 2020 Score Calculator
Calculates scores for book scanning problem solutions.
"""

import os
import sys
import argparse
from pathlib import Path


def read_instance(filepath):
    """Read a Hash Code instance file and return parsed data."""
    with open(filepath, 'r') as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]
    
    # First line: B L D
    B, L, D = map(int, lines[0].split())
    
    # Second line: book scores
    book_scores = list(map(int, lines[1].split()))
    
    # Libraries
    libraries = []
    line_idx = 2
    
    for i in range(L):
        # Library info line: N T M
        N, T, M = map(int, lines[line_idx].split())
        line_idx += 1
        
        # Books in library
        books = list(map(int, lines[line_idx].split()))
        line_idx += 1
        
        libraries.append({
            'books_count': N,
            'signup_time': T,
            'books_per_day': M,
            'books': set(books),  # Use set for O(1) lookup
            'books_list': books   # Keep original order if needed
        })
    
    return {
        'B': B,
        'L': L, 
        'D': D,
        'book_scores': book_scores,
        'libraries': libraries
    }


def read_solution(filepath):
    """Read a Hash Code solution file and return parsed data."""
    with open(filepath, 'r') as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]
    
    # First line: number of libraries
    A = int(lines[0])
    
    # Libraries to sign up
    solution_libraries = []
    line_idx = 1
    
    for i in range(A):
        # Library line: Y K
        Y, K = map(int, lines[line_idx].split())
        line_idx += 1
        
        # Books to scan
        books = list(map(int, lines[line_idx].split()))
        line_idx += 1
        
        solution_libraries.append({
            'library_id': Y,
            'books_count': K,
            'books': books
        })
    
    return {
        'libraries_count': A,
        'libraries': solution_libraries
    }


def calculate_score(instance, solution):
    """Calculate the score for a given instance and solution."""
    try:
        scanned_books = set()
        total_score = 0
        current_signup_day = 0
        
        # Process libraries in submission order
        for sol_lib in solution['libraries']:
            lib_id = sol_lib['library_id']
            
            # Validate library ID
            if lib_id >= len(instance['libraries']):
                raise ValueError(f"Invalid library ID: {lib_id}")
            
            instance_lib = instance['libraries'][lib_id]
            
            # Check if we have enough books in the solution
            if instance_lib['books_count'] < sol_lib['books_count']:
                raise ValueError(f"Library {lib_id} has only {instance_lib['books_count']} books, "
                               f"but solution requests {sol_lib['books_count']}")
            
            # Add signup time
            current_signup_day += instance_lib['signup_time']
            
            # If signup finishes after deadline, ignore this and all subsequent libraries
            if current_signup_day > instance['D']:
                break
            
            # Books can be scanned starting the day after signup completes
            scanning_start_day = current_signup_day
            remaining_days = instance['D'] - scanning_start_day
            
            # Scan books from this library
            for book_index, book_id in enumerate(sol_lib['books']):
                # Check if book exists in this library
                if book_id not in instance_lib['books']:
                    raise ValueError(f"Book {book_id} not available in library {lib_id}")
                
                # Check if we have time to scan this book
                # book_index / books_per_day gives the day when this book would be scanned
                days_needed = book_index // instance_lib['books_per_day']
                
                if days_needed >= remaining_days:
                    break  # No more time to scan remaining books from this library
                
                # If book hasn't been scanned yet, add its score
                if book_id not in scanned_books:
                    scanned_books.add(book_id)
                    total_score += instance['book_scores'][book_id]
        
        return total_score, len(scanned_books)
        
    except Exception as e:
        return f"Error: {e}", 0


def find_matching_files(instances_dir, solutions_dir):
    """Find matching instance and solution file pairs."""
    instance_files = {}
    for file_path in instances_dir.glob("*.txt"):
        base_name = file_path.stem
        instance_files[base_name] = file_path
    
    solution_files = {}
    for file_path in solutions_dir.glob("*.out"):
        base_name = file_path.stem
        solution_files[base_name] = file_path
    
    # Find pairs
    pairs = []
    for base_name in sorted(instance_files.keys()):
        if base_name in solution_files:
            pairs.append((base_name, instance_files[base_name], solution_files[base_name]))
        else:
            print(f"Warning: No solution found for instance {base_name}")
    
    # Check for solutions without matching instances
    for base_name in solution_files.keys():
        if base_name not in instance_files:
            print(f"Warning: No instance found for solution {base_name}")
    
    return pairs


def main():
    instances_dir = 'instances'
    solutions_dir = 'output/d'
    verbose = False

    instances_dir = Path(instances_dir)
    solutions_dir = Path(solutions_dir)
    
    if not instances_dir.exists():
        print(f"Error: Instances directory not found: {instances_dir}")
        sys.exit(1)
    
    if not solutions_dir.exists():
        print(f"Error: Solutions directory not found: {solutions_dir}")
        sys.exit(1)
    
    # Find matching files
    pairs = find_matching_files(instances_dir, solutions_dir)
    
    if not pairs:
        print("No matching instance-solution pairs found!")
        sys.exit(1)
    
    print("Hash Code 2020 Book Scanning - Score Calculator")
    print("=" * 60)
    
    total_score = 0
    total_books = 0
    processed_count = 0
    
    for base_name, instance_file, solution_file in pairs:        
        try:
            # Read files
            instance = read_instance(instance_file)
            solution = read_solution(solution_file)
            
            # Calculate score
            result = calculate_score(instance, solution)
            
            if isinstance(result[0], int):
                score, books_scanned = result
                if verbose:
                    print(f"  Books scanned: {books_scanned}")
                    print(f"{score}")
                else:
                    print(f"{base_name} {score}")
                
                total_score += score
                total_books += books_scanned
                processed_count += 1
            else:
                error_msg = result[0]
                if verbose:
                    print(f"  {error_msg}")
                else:
                    print(f"Error: {error_msg}")
                
        except Exception as e:
            error_msg = f"Failed to process: {e}"
            if verbose:
                print(f"  {error_msg}")
            else:
                print(error_msg)
    
    print("=" * 60)
    print(f"Files processed successfully: {processed_count}/{len(pairs)}")
    print(f"Total books scanned: {total_books:,}")
    print(f"Total score: {total_score:,}")
    
    if processed_count > 0:
        avg_score = total_score / processed_count
        print(f"Average score per instance: {avg_score:,.1f}")


if __name__ == "__main__":
    main()