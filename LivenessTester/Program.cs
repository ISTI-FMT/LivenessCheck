using LivenessCheck;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LivenessTester
{
    class Program
    {
        /// <summary>
        /// Ritorna una lista di missioni (lista di ID di CDB) a partire da un file di testo
        /// </summary>
        private static List<MissioneLiveness> CaricaMissioni(string nomefile)
        {
            List<MissioneLiveness> missioni = new List<MissioneLiveness>();

            if (!File.Exists(nomefile))
                return missioni;

            FileStream stream = null;
            StreamReader sr = null;
            try
            {
                stream = File.Open(nomefile, FileMode.Open, FileAccess.Read);
                sr = new StreamReader(stream);

                while (!sr.EndOfStream)
                {
                    string line = sr.ReadLine();

                    //Formato di una riga
                    // nometreno = [ x,y,z ]
                    if (string.IsNullOrEmpty(line) || line[0] == '#')
                        continue;

                    string[] tokens = line.Split('=');
                    if (tokens.Length > 1)
                    {
                        string nometreno = tokens[0].Trim();

                        string cdb = tokens[1].TrimStart(new[] { '[', ' ' });
                        cdb = cdb.TrimEnd(new[] { ']', ' ' });
                        cdb = cdb.Replace(" ", "");
                        List<string> cdbList = cdb.Split(',').ToList();
                        List<int> cdbListInt = cdbList.ConvertAll(Convert.ToInt32);

                        int trn;
                        if (!int.TryParse(nometreno, out trn))
                        {
                            trn = missioni.Count;   
                        }
                        MissioneLiveness missione = new MissioneLiveness(trn);
                        missione.Cdbs = cdbListInt.ToArray();
                        missioni.Add(missione);

                        Console.WriteLine("{0}= [{1}]", nometreno, cdb);
                    }
                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
            finally
            {
                if (sr != null)
                {
                    sr.Close();
                    sr.Dispose();
                }
                if (stream != null)
                {
                    stream.Close();
                    stream.Dispose();
                }
            }
            return missioni;
        }

        static void Main(string[] args)
        {
            LivenessCheck.Liveness liveness = new LivenessCheck.Liveness(new int[] {});
            
            if (args.Length < 1)
            {
                Console.WriteLine("Utilizzo: LivenessTester <nomefile>");
                return;
            }
            
            List<MissioneLiveness> missioni = CaricaMissioni(args[0]);
            foreach (MissioneLiveness missione in missioni)
            {
                liveness.AggiungiMissione(missione);
            }

            DateTime start = DateTime.Now;
            Stack<Movimento> sequenza = liveness.LivenessCheck(missioni[0].Trn, missioni[0].Cdbs[0], false);
            DateTime end = DateTime.Now;

            bool isliveness = sequenza != null;

            TimeSpan elapsed = end.Subtract(start);

            Console.WriteLine();
            Console.WriteLine("Liveness: {0}", liveness);

            if (isliveness)
            {
                Console.Write("Movements (Trn,Cdb): ");
                StringBuilder sb = new StringBuilder();
                while (sequenza.Count > 0)
                {
                    Movimento top = sequenza.Pop();
                    sb.Append(top.Trn + "=>" + top.Cdb);
                    sb.Append(", ");
                }
                Console.WriteLine(sb.ToString().Trim(new[] { ',', ' ' }));
            }

            Console.WriteLine("Elapsed time: {0} seconds", elapsed.TotalSeconds);
        }
    }
}
