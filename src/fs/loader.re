module type T = {
  let fs_name: string;
  let file_read: Fp.t(Fp.absolute) => CCResult.t(string, string);
  let file_exists: Fp.t(Fp.absolute) => bool;
};
