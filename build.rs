use cmake;

fn main() {
    let dst = cmake::build("emulator");

    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=emulator");
    println!("cargo:rustc-link-lib=static=stdc++");
}

